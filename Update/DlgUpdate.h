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
    void sigDownLoadStart(bool bPrompt);
    void sigDownLoadEnd(int nErr);
    void sigError(int nErr, const QString &szErr);
    void sigProcess(double nTotal, double nNow);

private slots:
    void slotDownLoadStart(bool bPrompt);
    void slotDownLoadEnd(int nErr);
    void slotError(int nErr, const QString &szErr);
    void slotProcess(double nTotal, double nNow);

    void on_pbOk_clicked();
    
    void on_pbCancel_clicked();
    
private:
    int DownloadFile();
    Ui::CDlgUpdate *ui;

    CDownLoad m_DownLoadVersion;
    //下载版本更新文件处理器  
    CDownLoadHandleVersionFile m_VersionFileHandle;
    CDownLoad m_DownLoadFile;
    CDownLoadHandleFile m_HandleDownLoad;

    bool m_bDownloading;//是否正在下载  
    QString m_szUrl, m_szVersionFile;
};

#endif // DLGUPDATE_H
