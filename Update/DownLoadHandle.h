#ifndef DOWNLOADHANDLE_H
#define DOWNLOADHANDLE_H

#include "DownLoad.h"
#include <QString>
#include <QProcess>
#include <QObject>

class CDlgUpdate;
class CDownLoadHandleFile : public CDownLoadHandle, QObject
{
public:
    CDownLoadHandleFile(CDlgUpdate* pDlgUpdate = NULL);
    ~CDownLoadHandleFile();
    int SetFile(const QString &szFile);
    int SetMd5sum(const QString &szMd5);
    virtual int OnProgress(double total, double now);
    virtual int OnEnd(int nErrorCode);
    virtual int OnError(int nErrorCode, const std::string &szErr);

private:
    CDlgUpdate* m_pDlg;
    QString m_szFile;//安装文件  
    QProcess m_Process;
    QString m_szFileMd5sum;//安装文件的md5校验和  
};

class CDownLoadHandleVersionFile : public CDownLoadHandle
{
public:
    CDownLoadHandleVersionFile(const std::string &szFile = std::string(), CDlgUpdate* pDlgUpdate = NULL);
    int SetFile(const std::string &szFile);

    //处理结束  
    virtual int OnEnd(int nErrorCode);
    virtual int OnError(int nErrorCode, const std::string &szErr);

private:
    CDlgUpdate* m_pDlg;
    std::string m_szFile;//版本文件  
};

#endif // DOWNLOADHANDLE_H
