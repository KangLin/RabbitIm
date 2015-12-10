#ifndef DOWNLOADHANDLE_H
#define DOWNLOADHANDLE_H

#include "DownLoad.h"
#include <QString>
#include <QProcess>
#include <QObject>
#include "Global/Global.h"

class CDlgUpdate;
class RABBITIM_SHARED_LIBRARY CDownLoadHandleFile : public CDownLoadHandle, QObject
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

class RABBITIM_SHARED_LIBRARY CDownLoadHandleVersionFile : public CDownLoadHandle
{
public:
    CDownLoadHandleVersionFile();

    //开始更新  
    int Start();

    //处理结束  
    virtual int OnEnd(int nErrorCode);
    virtual int OnError(int nErrorCode, const std::string &szErr);

private:
    //下载版本更新  
    CDownLoad m_DownLoad;
    QString m_szFile;//版本文件  
};

#endif // DOWNLOADHANDLE_H
