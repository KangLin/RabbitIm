/**
 * @brief 多线程下载类,此类遵守c++11标准,可以单独使用  
 * @author 康林  
 * @date 2014-12-08
 *
 * @class CDownLoad DownLoad.h "Update/DownLoad.h"
 * 注意:CDownLoad实例生命周期大于下载的生命周期,否则会阻塞调用线程.因为在析构函数中会等待所有线程退出  
 * 使用:   
 *     //从 CDownLoadHandle 派生一个类,自定义一个处理实例  
 *     CDownLoadHandle h; 
 *     CDownLoad d; //定义实例  
 *     d.Start("http://182.254.185.29/a", "a", &h, 20);
 */

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>

//下载处理类  
class CDownLoadHandle
{
public:
    CDownLoadHandle();
    virtual ~CDownLoadHandle();

    //处理进度  
    //参数:  
    //        total:文件总长度  
    //        now:已下载长度  
    virtual int OnProgress(double total, double now);
    //处理结束  
    virtual int OnEnd(int nErrorCode);
};

class CDownLoad
{
public:
    CDownLoad(const std::string &szUrl = std::string(), const std::string &szFile = std::string(), CDownLoadHandle* pHandle = NULL);
    virtual ~CDownLoad();
    int Start(const std::string &szUrl = std::string(), const std::string &szFile = std::string(), CDownLoadHandle *pHandle = NULL, int nNumThread = 10);
    int Start(const char* pUrl, const char* pFile, CDownLoadHandle *pHandle = NULL, int nNumThread = 10);

    /**
     * @brief 得到文件的长度  
     *
     * @fn GetFileLength
     * @param szFile
     * @return long
     */
    double GetFileLength(const std::string &szFile);
    int GetRange(long &nStart, long &nEnd);
    static size_t Write(void *buffer, size_t size, size_t nmemb, void *para);
    static int Work(void *pPara);
    static int Main(void *pPara);
    static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow); 
    std::string m_szUrl;                       //下载地址  
    std::string m_szFile;                      //保存到本地文件  
	std::ofstream m_streamFile;      //本地文件流  
    double m_dbFileLength;             //文件总长度  
    double m_dbAlready;                   //已经下载的长度  
    std::mutex m_MutexAlready;    //已经下载的长度互斥量  
    double m_nBlockSize;                  //一个线程下载块的大小  
    const int m_nBlockSizeMin;                 //块的最小值  
    double m_nDownLoadPostion; //将要下载的位置  
    int m_nNumberThreads;             //下载线程数  
    std::thread m_MainThread;       //下载主线程,用于等待同步下载线程  
    std::mutex m_Mutex;
    CDownLoadHandle* m_pHandle;
    int m_nErrorCode;
};

#endif // DOWNLOAD_H
