#include "Log.h"
#include <string>
#include <stdarg.h>
#include <QDebug>

CLog::CLog()
{
}

CLog* CLog::Instance()
{
    static CLog* p = NULL;
    if(!p)
        p = new CLog;
    return p;
}

#define LOG_BUFFER_LENGTH 1024
int CLog::Log(const char *pszFile, int nLine, int nLevel,
                 const char* pszModelName, const char *pFormatString, ...)
{
    char buf[LOG_BUFFER_LENGTH];
    std::string szTemp = pszFile;
    szTemp += "(";
    sprintf(buf, "%d", nLine);
    szTemp += buf;
    szTemp += "):";
    switch(nLevel)
    {
    case LM_DEBUG:
        szTemp += "DEBUG";
        break;
    case LM_ERROR:
        szTemp += "ERROR";
        break;
    case LM_INFO:
        szTemp += "INFO";
        break;
    case LM_WARNING:
        szTemp = "WARNING";
        break;
    }
    szTemp += ":";
    szTemp += pszModelName;
    szTemp += ":";

    va_list args;
    va_start (args, pFormatString);
    int nRet = vsnprintf(buf, LOG_BUFFER_LENGTH, pFormatString, args);
    va_end (args);
    if(nRet < 0 || nRet >= LOG_BUFFER_LENGTH)
    {
        LOG_MODEL_ERROR("Global",
                        "vsprintf buf is short, %d > %d. Truncated it:%d",
                        nRet > LOG_BUFFER_LENGTH, LOG_BUFFER_LENGTH);
        buf[LOG_BUFFER_LENGTH - 1] = 0;
        //return nRet;
    }
    szTemp += buf;

    qDebug() << szTemp.c_str();
    return 0;
}
