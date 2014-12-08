#include "DownLoad.h"
#include <stdio.h>
#include <curl/curl.h>
#include <sstream>

#ifdef RABBITIM_USER_LIBCURL
	#include "Global/Global.h"
#else
	#define LOG_MODEL_ERROR(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
	#define LOG_MODEL_DEBUG(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif

CDownLoadHandle::CDownLoadHandle()
{}

CDownLoadHandle::~CDownLoadHandle()
{}

int CDownLoadHandle::OnProgress(double total, double now)
{
    LOG_MODEL_DEBUG("CDownLoadHandle", "Progress:%f%%", 100 * now / total);
    return 0;
}

int CDownLoadHandle::OnEnd(int nErrorCode)
{
    LOG_MODEL_DEBUG("CDownLoadHandle", "End.errcode:%d", nErrorCode);
    return 0;
}

//文件写入回调函数的数据结构  
struct _FILE_STRUCT {
  long start;
  long end;
  CDownLoad* pThis;
};

//处理回调函数的数据结构  
struct _PROCESS_STRUCT{
    long nAlready;//已经下载过的长度  
    CDownLoad* pThis;
};

CDownLoad::CDownLoad(const std::string &szUrl, const std::string &szFile, CDownLoadHandle *pHandle)
{
    m_szUrl = szUrl;
    m_szFile = szFile;
    m_dbFileLength = 0;
    m_nDownLoadPostion = 0;
    m_dbAlready = 0;
    m_nBlockSize = 0;
    m_nNumberThreads = 0;
    m_pHandle = pHandle;
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CDownLoad::~CDownLoad()
{
    m_MainThread.join();
    curl_global_cleanup();
}

size_t CDownLoad::Write(void *buffer, size_t size, size_t nmemb, void *para)
{
  struct _FILE_STRUCT *out = (struct _FILE_STRUCT *)para;
  if(!out || !out->pThis->m_streamFile || out->start >= out->end)
  {
      return -1; /* failure, can't open file to write */
  }
  LOG_MODEL_ERROR("CDownLoad", "write:size%d,start:%d,end:%d", size * nmemb, out->start, out->end);
  size_t nWrite = 0;
  if(size * nmemb > out->end - out->start + 1)
  {
      //理论上不会进入这里   
      LOG_MODEL_ERROR("CDownLoad", "buffer Greater than end");
      out->pThis->m_Mutex.lock();
      out->pThis->m_streamFile.seekp(out->start, std::ios::beg);
      nWrite = out->pThis->m_streamFile.write((const char*)buffer, out->end - out->start + 1).tellp() - out->start;
      out->start += nWrite;
      out->pThis->m_Mutex.unlock();
  }
  else
  {
      out->pThis->m_Mutex.lock();
      out->pThis->m_streamFile.seekp(out->start, std::ios::beg);
      nWrite = out->pThis->m_streamFile.write((const char*)buffer, size * nmemb).tellp() - out->start;
      out->start += nWrite;
      out->pThis->m_Mutex.unlock();
  }
  LOG_MODEL_ERROR("CDownLoad", "write:size:%d", nWrite);
  return nWrite;
}

/************************************************************************/
/* 获取要下载的远程文件的大小                                                */
/************************************************************************/
double CDownLoad::GetFileLength(const std::string &szFile)
{
    double nLength = 0;
    CURL *pCurl;
    pCurl = curl_easy_init();
    if(!pCurl)
        return -1;
    curl_easy_setopt (pCurl, CURLOPT_URL, szFile.c_str());
    curl_easy_setopt (pCurl, CURLOPT_HEADER, 1);    //只需要header头  
    curl_easy_setopt (pCurl, CURLOPT_NOBODY, 1);    //不需要body  
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 20);        //设置超时  
    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号  
#ifdef DEBUG
        /* Switch on full protocol/debug output */
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
    if (curl_easy_perform (pCurl) == CURLE_OK)
    {
        curl_easy_getinfo (pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &nLength);
        LOG_MODEL_DEBUG("CDownLoad", "length:%f", nLength);
    }
    curl_easy_cleanup(pCurl);
    return nLength;
}

int CDownLoad::GetRange(long &nStart, long &nEnd)
{
    if(m_nDownLoadPostion >= m_dbFileLength)
        return -1;

    m_Mutex.lock();
    nStart = m_nDownLoadPostion;
    if(m_nDownLoadPostion + m_nBlockSize > m_dbFileLength)
        nEnd = nStart + m_dbFileLength - m_nDownLoadPostion - 1;
    else
        nEnd = nStart + m_nBlockSize - 1;
    m_nDownLoadPostion = nEnd + 1;
    m_Mutex.unlock();

    return 0;
}

int CDownLoad::Work(void *pPara)
{
    CURLcode res;
    _FILE_STRUCT file{0};
    _PROCESS_STRUCT process{0};
    CDownLoad* pDownLoad = (CDownLoad*)pPara;
    file.pThis = pDownLoad;
    process.pThis = pDownLoad;
    while(0 == pDownLoad->GetRange(file.start, file.end))
    {
        std::string szRange;
        char range[64] = { 0 };
       #ifdef WIN32
       _snprintf_s(range, sizeof (range), "%ld-%ld", file.start, file.end);
       #else
       snprintf (range, sizeof (range), "%ld-%ld", file.start, file.end);
       #endif
       szRange = range;

        CURL *pCurl;
        pCurl = curl_easy_init();
        if(!pCurl)
            return -1;

        /*
       * You better replace the URL with one that works!
       */
        curl_easy_setopt(pCurl, CURLOPT_URL,
                         pDownLoad->m_szUrl.c_str());
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CDownLoad::Write);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &file);
        //curl_easy_setopt (pCurl, CURLOPT_LOW_SPEED_LIMIT, 1L);  
        //curl_easy_setopt (pCurl, CURLOPT_LOW_SPEED_TIME, 5L);  
        //设置下载区间  
        curl_easy_setopt (pCurl, CURLOPT_RANGE, szRange.c_str());
        //设置处理进度函数  
        curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, CDownLoad::progress_callback);
        curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, &process);

#ifdef DEBUG
        /* Switch on full protocol/debug output */
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
        res = curl_easy_perform(pCurl);

        /* always cleanup */
        curl_easy_cleanup(pCurl);

        if(CURLE_OK != res) {
            /* we failed */
            LOG_MODEL_ERROR("CDownLoad", "curl told us %d\n", res);
        }
    }

    if(0 == pDownLoad->m_nErrorCode)
        pDownLoad->m_nErrorCode = res;

    return 0;
}

int CDownLoad::progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    _PROCESS_STRUCT* p = (_PROCESS_STRUCT*)clientp;
    CDownLoad* pThis = p->pThis;
    if(NULL == pThis->m_pHandle)
        return 0;
    pThis->m_MutexAlready.lock();
    pThis->m_dbAlready += (dlnow - p->nAlready);
    pThis->m_MutexAlready.unlock();
    p->nAlready = dlnow;
    return pThis->m_pHandle->OnProgress(pThis->m_dbFileLength, pThis->m_dbAlready);
}

int CDownLoad::Start(const char *pUrl, const char *pFile, CDownLoadHandle *pHandle, int nNumThread)
{
    std::string szUrl(pUrl), szFile(pFile);
    return Start(szUrl, szFile, pHandle, nNumThread);
}

int CDownLoad::Start(const std::string &szUrl, const std::string &szFile, CDownLoadHandle *pHandle, int nNumThread)
{
    if(!szUrl.empty())
        m_szUrl = szUrl;
    if(m_szUrl.empty())
        return -1;

    if(!szFile.empty())
        m_szFile = szFile;
    if(m_szFile.empty())
        return -2;
    if(!m_streamFile)
	{
        m_streamFile.close();
        m_streamFile.clear();
	}

    m_dbFileLength = GetFileLength(m_szUrl);
    if(m_dbFileLength <= 0)
        return -3;
    m_nBlockSize = m_dbFileLength / nNumThread;

    m_nDownLoadPostion = 0;
    m_dbAlready = 0;
    m_nNumberThreads = nNumThread;

    if(pHandle)
        m_pHandle = pHandle;

    //打开文件  
	m_streamFile.open(m_szFile.c_str());
	if (!m_streamFile)
		return -4;

    m_MainThread = std::thread(Main, this);

    return 0;
}

int CDownLoad::Main(void *pPara)
{
    CDownLoad* p = (CDownLoad*)pPara;
    std::vector<std::thread> threads;
    //启动线程  
    for(int i = 0; i < p->m_nNumberThreads; i++)
        threads.push_back(std::thread(Work, p));

    for (auto& th : threads) th.join();
	p->m_streamFile.close();
	p->m_streamFile.clear();

    LOG_MODEL_DEBUG("CDownLoad", "download end");
}
