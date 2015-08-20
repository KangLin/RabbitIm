#include "DownLoad.h"
#include <stdio.h>
#include <sstream>

#ifdef RABBITIM_USE_LIBCURL
    #include "Global/Global.h"
#else
    #define LOG_MODEL_ERROR(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
    #define LOG_MODEL_DEBUG(mod, fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif

#define SKIP_PEER_VERIFICATION 1
#define SKIP_HOSTNAME_VERIFICATION 1
CDownLoadHandle::CDownLoadHandle()
{}

CDownLoadHandle::~CDownLoadHandle()
{}

int CDownLoadHandle::OnProgress(double total, double now)
{
    LOG_MODEL_DEBUG("CDownLoad", "thread id:0x%X; total:%f,now:%f;Progress:%f%%", 
                    std::this_thread::get_id(),
                    total,
                    now,
                    100 * now / total);
    return 0;
}

int CDownLoadHandle::OnEnd(int nErrorCode)
{
    LOG_MODEL_DEBUG("CDownLoad", "End.errcode:%d", nErrorCode);
    return 0;
}

int CDownLoadHandle::OnError(int nErrorCode, const std::string &szErr)
{
    LOG_MODEL_ERROR("CDownLoad", "DownLoad error.[%d]%s", nErrorCode, szErr.c_str());
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
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_pMainThread = NULL;
    m_nNumberReWhile = 3;
    Init();
    m_szUrl = szUrl;
    m_szFile = szFile;
    m_pHandle = pHandle;
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
        delete m_pMainThread;
        m_pMainThread = NULL;
    }
    curl_global_cleanup();
}

int CDownLoad::Init()
{
    if(m_pMainThread &&m_pMainThread->joinable())
    {
        m_pMainThread->join();
        delete m_pMainThread;
        m_pMainThread = NULL;
    }
    m_dbFileLength = 0;
    m_nDownLoadPostion = 0;
    m_dbAlready = 0;
    m_nBlockSize = 0;
    m_nNumberThreads = 1;
    m_nErrorCode = 0;
    m_bExit = false;
    m_nTimeOut = 20;
    return 0;
}

size_t CDownLoad::Write(void *buffer, size_t size, size_t nmemb, void *para)
{
  struct _FILE_STRUCT *out = (struct _FILE_STRUCT *)para;
  if(!out ||
      !out->pThis ||
      !out->pThis->m_streamFile
      )
  {
      LOG_MODEL_ERROR("CDownLoad", "threadid:0x%X",
                      std::this_thread::get_id());
      return -1; /* failure, can't open file to write */
  }
  LOG_MODEL_DEBUG("CDownLoad", "threadid:0x%X;write:size:%d,start:%d,end:%d",
                  std::this_thread::get_id(), size * nmemb, out->start, out->end);

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

  LOG_MODEL_DEBUG("CDownLoad", "threadid:0x%X;write:size:%d", std::this_thread::get_id(), nWrite);
  return nWrite;
}

size_t CDownLoad::WriteSingle(void *buffer, size_t size, size_t nmemb, void *para)
{
    struct _FILE_STRUCT *out = (struct _FILE_STRUCT *)para;
    if(!out ||
        !out->pThis ||
        !out->pThis->m_streamFile
        )
    {
        return -1; /* failure, can't open file to write */
    }

    CDownLoad* pThis = out->pThis;
    if(pThis->m_bExit)
        return -2;

    size_t nWrite = size * nmemb;
    std::streamsize nSize = nWrite;

    pThis->m_streamFile.write((const char*)buffer, nSize);

    return nWrite;
}

/************************************************************************/  
/* 获取要下载的远程文件的大小                                                */  
/************************************************************************/ 
double CDownLoad::GetFileLength(const std::string &szUrl)
{
    double nLength = 0;
    CURL *pCurl;
    pCurl = curl_easy_init();
    if(!pCurl)
    {
        if(m_pHandle)
            m_pHandle->OnError(ERROR_CURL, "curl init error");
        return -1;
    }
    curl_easy_setopt (pCurl, CURLOPT_URL, szUrl.c_str());
    curl_easy_setopt (pCurl, CURLOPT_HEADER, 1);    //只需要header头  
    curl_easy_setopt (pCurl, CURLOPT_NOBODY, 1);    //不需要body  
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, m_nTimeOut);   //设置超时  
    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);   //屏蔽其它信号  
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */ 
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
 
#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */ 
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
#ifdef DEBUG
        /* Switch on full protocol/debug output */
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
    if (curl_easy_perform (pCurl) == CURLE_OK)
    {
        curl_easy_getinfo (pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &nLength);
        LOG_MODEL_DEBUG("CDownLoad", "length:%f", nLength);
    }
    else
    {
        if(m_pHandle)
            m_pHandle->OnError(ERROR_GET_FILE_LENGTH, "Get file length error." + szUrl);
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
        int nNumber = pDownLoad->m_nNumberReWhile;//出错重试次数  
        do
        {
            unsigned long start = file.start, end = file.end;
            LOG_MODEL_DEBUG("CDownLoad", "threadid:0x%X;nNumber:%d;start:%d;end:%d", std::this_thread::get_id(), nNumber, start, end);
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
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, pDownLoad->m_nTimeOut);   //设置超时  
            //curl_easy_setopt (pCurl, CURLOPT_LOW_SPEED_LIMIT, 1L);  
            //curl_easy_setopt (pCurl, CURLOPT_LOW_SPEED_TIME, 5L);  
            //设置下载区间  
            curl_easy_setopt (pCurl, CURLOPT_RANGE, szRange.c_str());
            //设置处理进度函数  
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
#if LIBCURL_VERSION_NUM >= 0x072000
            /* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
           compile as they won't have the symbols around.
           
           If built with a newer libcurl, but running with an older libcurl:
           curl_easy_setopt() will fail in run-time trying to set the new
           callback, making the older callback get used.
           
           New libcurls will prefer the new callback and instead use that one even
           if both callbacks are set. */ 
            
            curl_easy_setopt(pCurl, CURLOPT_XFERINFOFUNCTION, xferinfo);
            /* pass the struct pointer into the xferinfo function, note that this is
           an alias to CURLOPT_PROGRESSDATA */ 
            curl_easy_setopt(pCurl, CURLOPT_XFERINFODATA, &process);
#else
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, CDownLoad::progress_callback);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, &process);
#endif
            
#ifdef SKIP_PEER_VERIFICATION
            /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */ 
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
            
#ifdef SKIP_HOSTNAME_VERIFICATION
            /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */ 
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
            
#ifdef DEBUG
            /* Switch on full protocol/debug output */
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
            res = curl_easy_perform(pCurl);
            
            /* always cleanup */
            curl_easy_cleanup(pCurl);
            if(CURLE_OK != res && 0 >= nNumber) {
                /* we failed */
                LOG_MODEL_ERROR("CDownLoad", "threadid:0x%X;curl perform error:%d;start:%d:end:%d\n",
                                std::this_thread::get_id(), res, start, end);
                if(pDownLoad->m_pHandle)
                    pDownLoad->m_pHandle->OnError(ERROR_DOWNLOAD_FILE, "Download file error");
                pDownLoad->m_bExit = true;
            }
        }while(CURLE_OK != res && !pDownLoad->m_bExit && nNumber--);
    }

    if(0 == pDownLoad->m_nErrorCode)
        pDownLoad->m_nErrorCode = res;

    return 0;
}

int CDownLoad::WorkSingle(void *pPara)
{
    CURLcode res = CURLE_OK;
    _FILE_STRUCT file{0};
    _PROCESS_STRUCT process{0};
    CDownLoad* pDownLoad = (CDownLoad*)pPara;
    file.pThis = pDownLoad;
    process.pThis = pDownLoad;

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
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CDownLoad::WriteSingle);
    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, pDownLoad->m_nTimeOut);   //设置超时  

    //设置处理进度函数  
    curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
#if LIBCURL_VERSION_NUM >= 0x072000
    /* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
       compile as they won't have the symbols around.
 
       If built with a newer libcurl, but running with an older libcurl:
       curl_easy_setopt() will fail in run-time trying to set the new
       callback, making the older callback get used.
 
       New libcurls will prefer the new callback and instead use that one even
       if both callbacks are set. */ 
 
    curl_easy_setopt(pCurl, CURLOPT_XFERINFOFUNCTION, xferinfo);
    /* pass the struct pointer into the xferinfo function, note that this is
       an alias to CURLOPT_PROGRESSDATA */ 
    curl_easy_setopt(pCurl, CURLOPT_XFERINFODATA, &process);
#else
    curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, CDownLoad::progress_callback);
    curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, &process);
#endif

#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */ 
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
    
#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */ 
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    
#ifdef DEBUG
    /* Switch on full protocol/debug output */
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
#endif
    res = curl_easy_perform(pCurl);
    
    /* always cleanup */
    curl_easy_cleanup(pCurl);
    
    if(CURLE_OK != res) {
        /* we failed */
        if(pDownLoad->m_pHandle)
            pDownLoad->m_pHandle->OnError(ERROR_DOWNLOAD_FILE, "Download file error");
    }
    

    if(0 == pDownLoad->m_nErrorCode)
        pDownLoad->m_nErrorCode = res;

    return 0;
}

int CDownLoad::progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    LOG_MODEL_DEBUG("CDwonLoad", "total:%f;now:%f;utotal:%f;unow:%f", dltotal, dlnow, ultotal, ulnow);
    _PROCESS_STRUCT* p = (_PROCESS_STRUCT*)clientp;
    CDownLoad* pThis = p->pThis;
    if(NULL == pThis->m_pHandle || dltotal <= 0)
        return 0;
    if(pThis->m_nNumberThreads == 1)
    {
        return pThis->m_pHandle->OnProgress(dltotal, dlnow);
    }

    pThis->m_MutexAlready.lock();
    pThis->m_dbAlready += (dlnow - p->nAlready);
    pThis->m_MutexAlready.unlock();
    p->nAlready = dlnow;
    return pThis->m_pHandle->OnProgress(pThis->m_dbFileLength, pThis->m_dbAlready);
}

int CDownLoad::xferinfo(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    /*LOG_MODEL_DEBUG("CDwonLoad", "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
                    "  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T,
                    ulnow, ultotal, dlnow, dltotal);*/
    _PROCESS_STRUCT* p = (_PROCESS_STRUCT*)clientp;
    CDownLoad* pThis = p->pThis;
    if(NULL == pThis->m_pHandle || dltotal <= 0)
        return 0;
    if(pThis->m_nNumberThreads == 1)
    {
        return pThis->m_pHandle->OnProgress(dltotal, dlnow);
    }
    
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

int CDownLoad::Start(const char *pUrl, const char *pFile, CDownLoadHandle *pHandle, int nNumThread, int nTimeOut)
{
    std::string szUrl(pUrl), szFile(pFile);
    return Start(szUrl, szFile, pHandle, nNumThread, nTimeOut);
}

int CDownLoad::Start(const std::string &szUrl, const std::string &szFile, CDownLoadHandle *pHandle, int nNumThread, int nTimeOut)
{
    Init();
    m_nNumberThreads = nNumThread;
    m_nTimeOut = nTimeOut;
    if(!szUrl.empty())
        m_szUrl = szUrl;
    if(m_szUrl.empty())
    {
        LOG_MODEL_ERROR("CDownLoad", "url is null");
        if(pHandle)
            pHandle->OnError(ERROR_DOWNLOAD_URL_IS_EMPTY, "url is empty");
        return -1;
    }
    if(!szFile.empty())
        m_szFile = szFile;
    if(m_szFile.empty())
    {
        LOG_MODEL_ERROR("CDownLoad", "file name is null");
        if(pHandle)
            pHandle->OnError(ERROR_DOWNLOAD_FILE_IS_EMPTY, "file is empty");
        return -2;
    }
    if(!m_streamFile)
    {
        m_streamFile.close();
        m_streamFile.clear();
    }

    if(pHandle)
        m_pHandle = pHandle;

    //打开文件  
    //注意：一定要以二进制模式打开，否则可能写入的数量大于缓存的数量  
    m_streamFile.open(m_szFile, std::ios_base::out | std::ios_base::trunc | std::ios::binary);
    if (!m_streamFile)
    {
        LOG_MODEL_ERROR("CDownLoad", "Open file error:%s", m_szFile.c_str());
        if(pHandle)
            pHandle->OnError(ERROR_OPEN_FILE, "Open file error:" + m_szFile);
        return -4;
    }

    m_pMainThread = new std::thread(Main, this);

    return 0;
}

int CDownLoad::Main(void *pPara)
{
    CDownLoad* p = (CDownLoad*)pPara;
    std::vector<std::thread> threads;
    if(1 < p->m_nNumberThreads)
    {
        p->m_dbFileLength = p->GetFileLength(p->m_szUrl);
        if(p->m_dbFileLength <= 0)
        {
            if(p->m_pHandle)
                p->m_pHandle->OnError(ERROR_GET_FILE_LENGTH, "Get file length error:" + p->m_szUrl);
            return -3;
        }
        //如果是小文件，就用单线程上载  
        if(p->m_dbFileLength <= p->m_nBlockSizeMin)
        {
            std::thread t(WorkSingle, p);
            if(t.joinable()) t.join();
        }
        else
        {
            p->m_nBlockSize = p->m_dbFileLength / (p->m_nNumberThreads - 1);
            if(p->m_nBlockSize < p->m_nBlockSizeMin)
                p->m_nBlockSize = p->m_nBlockSizeMin;
            //启动线程  
            for(int i = 0; i < p->m_nNumberThreads; i++)
                threads.push_back(std::thread(Work, p));

            for (auto& th : threads) th.join();
        }
    }
    else
    {
        std::thread t(WorkSingle, p);
        if(t.joinable()) t.join();
    }

    p->m_streamFile.close();
    p->m_streamFile.clear();

    if(p->m_pHandle)
        p->m_pHandle->OnEnd(p->m_nErrorCode);

    LOG_MODEL_DEBUG("CDownLoad", "download end");
    return 0;
}
