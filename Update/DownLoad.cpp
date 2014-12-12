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
    LOG_MODEL_DEBUG("CDownLoadHandle", "thread id:0x%X; Progress:%f%%", std::this_thread::get_id(), 100 * now / total);
    return 0;
}

int CDownLoadHandle::OnEnd(int nErrorCode)
{
    LOG_MODEL_DEBUG("CDownLoadHandle", "End.errcode:%d", nErrorCode);
    return 0;
}

//文件写入回调函数的数据结构  
struct _FILE_STRUCT {
  unsigned long start;
  unsigned long end;
  CDownLoad* pThis;
};

//处理回调函数的数据结构  
struct _PROCESS_STRUCT{
    long nAlready;//已经下载过的长度  
    CDownLoad* pThis;
};

CDownLoad::CDownLoad(const std::string &szUrl, const std::string &szFile, CDownLoadHandle *pHandle):
    m_nBlockSizeMin(10240)
{
    m_pMainThread = NULL;
    Init();
    m_szUrl = szUrl;
    m_szFile = szFile;
    m_pHandle = pHandle;
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CDownLoad::~CDownLoad()
{
    if(!m_streamFile)
	{
        m_streamFile.close();
        m_streamFile.clear();
	}

    if(m_pMainThread &&m_pMainThread->joinable())
    {
        m_pMainThread->join();
        m_pMainThread = NULL;
    }
    curl_global_cleanup();
}

int CDownLoad::Init()
{
    if(m_pMainThread &&m_pMainThread->joinable())
    {
        m_pMainThread->join();
        m_pMainThread = NULL;
    }
    m_dbFileLength = 0;
    m_nDownLoadPostion = 0;
    m_dbAlready = 0;
    m_nBlockSize = 0;
    m_nNumberThreads = 0;
    m_nErrorCode = 0;
    m_bExit = false;
    return 0;
}

size_t CDownLoad::Write(void *buffer, size_t size, size_t nmemb, void *para)
{
  struct _FILE_STRUCT *out = (struct _FILE_STRUCT *)para;
  if(!out ||
	  !out->pThis ||
	  !out->pThis->m_streamFile || 
	  out->start >= out->end
	  )
  {
      return -1; /* failure, can't open file to write */
  }
  LOG_MODEL_ERROR("CDownLoad", "write:size%d,start:%d,end:%d",
                  size * nmemb, out->start, out->end);

  CDownLoad* pThis = out->pThis;
  if(pThis->m_bExit)
      return -2;

  std::streamoff pos;
  size_t nWrite = size * nmemb;
  std::streamsize nSize = nWrite;
  if(nWrite > out->end - out->start + 1)
  {
      //理论上不会进入这里   
      LOG_MODEL_ERROR("CDownLoad", "buffer Greater than end");
  }

  pThis->m_Mutex.lock();
  pThis->m_streamFile.seekp(out->start, std::ios::beg);
  pos = pThis->m_streamFile.write((const char*)buffer, nSize).tellp();
  pThis->m_Mutex.unlock();

  nWrite = pos - out->start;
  out->start += nWrite;

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
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);   //设置超时  
    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);   //屏蔽其它信号  
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

int CDownLoad::GetRange(unsigned long &nStart, unsigned long &nEnd)
{
    m_Mutex.lock();
    if(m_nDownLoadPostion >= m_dbFileLength)
    {
        m_Mutex.unlock();
        return -1;
    }

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
    CURLcode res = CURLE_OK;
    _FILE_STRUCT file{0};
    _PROCESS_STRUCT process{0};
    CDownLoad* pDownLoad = (CDownLoad*)pPara;
    file.pThis = pDownLoad;
    process.pThis = pDownLoad;
    while(0 == pDownLoad->GetRange(file.start, file.end) && !pDownLoad->m_bExit)
    {
        unsigned long start = file.start, end = file.end;
        LOG_MODEL_DEBUG("CDownLoad", "thread id:0x%X", std::this_thread::get_id());
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
            LOG_MODEL_ERROR("CDownLoad", "curl perform error:%d;start:%d:end:%d\n", res, start, end);
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

int CDownLoad::Exit()
{
    m_bExit = true;
    return 0;
}

int CDownLoad::Start(const char *pUrl, const char *pFile, CDownLoadHandle *pHandle, int nNumThread)
{
    std::string szUrl(pUrl), szFile(pFile);
    return Start(szUrl, szFile, pHandle, nNumThread);
}

int CDownLoad::Start(const std::string &szUrl, const std::string &szFile, CDownLoadHandle *pHandle, int nNumThread)
{
    Init();
    if(!szUrl.empty())
        m_szUrl = szUrl;
    if(m_szUrl.empty())
    {
        LOG_MODEL_ERROR("CDownLoad", "url is null");
        return -1;
    }
    if(!szFile.empty())
        m_szFile = szFile;
    if(m_szFile.empty())
    {
        LOG_MODEL_ERROR("CDownLoad", "file name is null");
        return -2;
    }
    if(!m_streamFile)
	{
        m_streamFile.close();
        m_streamFile.clear();
	}

    m_dbFileLength = GetFileLength(m_szUrl);
    if(m_dbFileLength <= 0)
        return -3;
    m_nBlockSize = m_dbFileLength / (nNumThread - 1);
    if(m_nBlockSize < m_nBlockSizeMin)
        m_nBlockSize = m_nBlockSizeMin;

    m_nDownLoadPostion = 0;
    m_dbAlready = 0;
    m_nNumberThreads = nNumThread;

    if(pHandle)
        m_pHandle = pHandle;

    //打开文件
    //注意：一定要以二进制模式打开，否则可能写入的数量大于缓存的数量  
	m_streamFile.open(m_szFile, std::ios_base::out | std::ios_base::trunc | std::ios::binary);
	if (!m_streamFile)
    {
        LOG_MODEL_ERROR("CDownLoad", "Open file error:%s", m_szFile.c_str());
        return -4;
    }

    m_pMainThread = new std::thread(Main, this);

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

    if(p->m_pHandle)
        p->m_pHandle->OnEnd(p->m_nErrorCode);

    LOG_MODEL_DEBUG("CDownLoad", "download end");
	return 0;
}
