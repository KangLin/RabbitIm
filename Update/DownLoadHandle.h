#ifndef DOWNLOADHANDLE_H
#define DOWNLOADHANDLE_H

#include "DownLoad.h"
#include <QString>
#include <QProcess>
#include <QObject>

class CDownLoadHandleFile : public CDownLoadHandle, QObject
{
public:
    CDownLoadHandleFile(QObject* parent = NULL);
    ~CDownLoadHandleFile();
    int SetFile(const QString &szFile);
    virtual int OnEnd(int nErrorCode);
private:
    QString m_szFile;
    QProcess m_Process;
};

class CDownLoadHandleVersionFile : public CDownLoadHandle, QObject
{
public:
    CDownLoadHandleVersionFile(const std::string &szFile = std::string(), QObject* parent = NULL);
    int SetFile(const std::string &szFile);

    //处理结束  
    virtual int OnEnd(int nErrorCode);

private:
    std::string m_szFile;//版本文件  
    CDownLoadHandleFile m_DownLoadFile;
    CDownLoad m_DownLoad;
};

#endif // DOWNLOADHANDLE_H
