#ifndef DLGUPDATE_H
#define DLGUPDATE_H

#include <QDialog>
#include "DownLoadHandle.h"

namespace Ui {
class CDlgUpdate;
}

class CDlgUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgUpdate(QWidget *parent = 0);
    ~CDlgUpdate();

    int Start();

signals:
    //版本文件下载后触发  
    void sigDownLoadVersionFile(int nErrorCode);
    //安装文件下载后触发  
    void sigDownLoadEnd(int nErr);
    //错误触发  
    void sigError(int nErr, const QString &szErr);
    //下载进度,默认一秒一次  
    void sigProcess(double nTotal, double nNow);

private slots:
    void slotDownLoadVersionFile(int nErrorCode);
    void slotDownLoadStart(bool bPrompt);
    void slotDownLoadEnd(int nErr);
    void slotError(int nErr, const QString &szErr);
    void slotProcess(double nTotal, double nNow);

    void on_pbOk_clicked();
    
    void on_pbCancel_clicked();
    
private:
    virtual void changeEvent(QEvent* e);
    int DownloadFile();
    Ui::CDlgUpdate *ui;

    //下载版本更新  
    CDownLoad m_DownLoadVersion;
    //下载版本更新文件处理器  
    CDownLoadHandleVersionFile m_VersionFileHandle;
    //下载安装文件  
    CDownLoad m_DownLoadFile;
    //下载安装文件处理器  
    CDownLoadHandleFile m_HandleDownLoad;

    bool m_bDownloading;//是否正在下载  
    QString m_szUrl, m_szVersionFile;
    
    QString m_szDownloadInfo;//新版本的信息  
    QString m_szDownloadMd5sum; //下载文件的MD5校验和  
    QString m_szDownLoadUrl;            //要下载文件的URL  
    QString m_szDownLoadFile;//下载文件存放本地的位置  
    QProcess m_Process;
};

#endif // DLGUPDATE_H
