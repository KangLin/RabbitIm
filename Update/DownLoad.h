#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>

/**
 * @brief 多线程下载类  
 *
 * @class CDownLoad DownLoad.h "Update/DownLoad.h"
 */
class CDownLoad
{
public:
    CDownLoad(const std::string &szUrl = std::string(), const std::string &szFile = std::string());
    virtual ~CDownLoad();
    int Start(const std::string &szUrl, const std::string &szFile, int nNumThread = 10, int nBlockSize = 10240);
    int Start(const char* pUrl, const char* pFile, int nNumThread = 10, int nBlockSize = 10240);

    /**
     * @brief 得到文件的长度  
     *
     * @fn GetFileLength
     * @param szFile
     * @return long
     */
    long GetFileLength(const std::string &szFile);
    int GetRange(long &nStart, long &nEnd);
    static size_t Write(void *buffer, size_t size, size_t nmemb, void *para);
    static int Work(void *pPara);
    std::string m_szUrl;
    std::string m_szFile;
	std::ofstream m_streamFile;
    long m_FileLength;
    int m_nBlockSize;
    int m_nDownLoadPostion;
    std::vector<std::thread> m_Threads;
    std::mutex m_Mutex;
};

#endif // DOWNLOAD_H
