#include "DownLoad.h"
#include <stdio.h>
#include <curl/curl.h>

#ifdef RABBITIM_USER_LIBCURL
	#include "Global/Global.h"
#else
	#define LOG_MODEL_ERROR(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
	#define LOG_MODEL_DEBUG(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif

struct _FILE_STRUCT {
  long start;
  long end;
  CDownLoad* pThis;
};

CDownLoad::CDownLoad(const std::string &szUrl, const std::string &szFile)
{
    m_szUrl = szUrl;
    m_szFile = szFile;
    m_FileLength = 0;
    m_nDownLoadPostion = 0;
    m_nBlockSize = 10240;
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CDownLoad::~CDownLoad()
{
    curl_global_cleanup();
}

size_t CDownLoad::Write(void *buffer, size_t size, size_t nmemb, void *para)
{
  struct _FILE_STRUCT *out=(struct _FILE_STRUCT *)para;
  if(!out || !out->pThis->m_streamFile)
  {
      return -1; /* failure, can't open file to write */
  }
  size_t nWrite = 0;
  if(out->start + size * nmemb > out->end)
  {
      LOG_MODEL_ERROR("CDownLoad", "buffer Greater than end");
      out->pThis->m_Mutex.lock();
      out->pThis->m_streamFile.seekp(out->start, std::ios::beg);
      nWrite = out->pThis->m_streamFile.write((const char*)buffer, out->end - out->start).tellp();
      out->pThis->m_Mutex.unlock();
      out->start += nWrite;
  }
  else
  {
      out->pThis->m_Mutex.lock();
      out->pThis->m_streamFile.seekp(out->start, std::ios::beg);
      nWrite = out->pThis->m_streamFile.write((const char*)buffer, size * nmemb).tellp();
      out->pThis->m_Mutex.unlock();
      out->start += nWrite;
  }
  return nWrite;
}

/************************************************************************/
/* 获取要下载的远程文件的大小                                                */
/************************************************************************/
long CDownLoad::GetFileLength(const std::string &szFile)
{
    long nLength = -1;
    CURL *pCurl;
    pCurl = curl_easy_init();
    if(!pCurl)
        return -1;
    curl_easy_setopt (pCurl, CURLOPT_URL, szFile.c_str());
    curl_easy_setopt (pCurl, CURLOPT_HEADER, 1);    //只需要header头  
    curl_easy_setopt (pCurl, CURLOPT_NOBODY, 1);    //不需要body  
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 20);        //设置超时  
    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号  
    if (curl_easy_perform (pCurl) == CURLE_OK)
    {
        curl_easy_getinfo (pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &nLength);
    }
    curl_easy_cleanup(pCurl);
    return nLength;
}

int CDownLoad::GetRange(long &nStart, long &nEnd)
{
    if(m_nDownLoadPostion >= m_FileLength)
        return -1;

    m_Mutex.lock();
    nStart = m_nDownLoadPostion;
    if(m_nDownLoadPostion + m_nBlockSize > m_FileLength)
        nEnd = m_FileLength - m_nDownLoadPostion;
    else
        nEnd = nStart + m_nBlockSize;
    m_nDownLoadPostion = nEnd;
    m_Mutex.unlock();
    
    return 0;
}

int CDownLoad::Work(void *pPara)
{
    _FILE_STRUCT file;
    CDownLoad* pDownLoad = (CDownLoad*)pPara;
    file.pThis = pDownLoad;
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
        
#ifdef DEBUG
        /* Switch on full protocol/debug output */
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
        CURLcode res = curl_easy_perform(pCurl);
        
        /* always cleanup */
        curl_easy_cleanup(pCurl);
        
        if(CURLE_OK != res) {
            /* we failed */
            LOG_MODEL_ERROR("CDownLoad", "curl told us %d\n", res);
        }
    }
    return 0;
}

int CDownLoad::Start(const char *pUrl, const char *pFile, int nNumThread, int nBlockSize)
{
    std::string szUrl(pUrl), szFile(pFile);
    return Start(szUrl, szFile, nNumThread, nBlockSize);
}

int CDownLoad::Start(const std::string &szUrl, const std::string &szFile, int nNumThread, int nBlockSize)
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

    m_FileLength = GetFileLength(m_szUrl);
    if(m_FileLength <= 0)
        return -3;
    m_nBlockSize = nBlockSize;
    m_nDownLoadPostion = 0;

    //打开文件  
	m_streamFile.open(m_szFile.c_str());
	if (!m_streamFile)
		return -4;

    //启动线程  
    for(int i = 0; i < nNumThread; i++)
        m_Threads.push_back(std::thread(Work, this));

    for (auto& th : m_Threads) th.join();
	m_streamFile.close();
	m_streamFile.clear();

    LOG_MODEL_DEBUG("CDownLoad", "download end");
    return 0;
}
