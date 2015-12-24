#include "DlgUpdate.h"
#include "ui_DlgUpdate.h"
#include "Tool.h"
#include "Version.h"
#include "Global/Global.h"
#include <QDir>
#include <QUrl>
#include <QtXml>
#include "MainWindow.h"

CDlgUpdate::CDlgUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUpdate),
    m_HandleDownLoad(this),
    m_Update(this)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);

    ui->lbCurrentVersion->setText(tr("Current version:")
                                  + QString::number(MAJOR_VERSION_NUMBER)
                                  + "."
                                  + QString::number(MINOR_VERSION_NUMBER)
                                  + "."
                                  + QString::number(REVISION_VERSION_NUMBER));
    ui->lbPrompt->setText(tr("The new version is not detected."));
    ui->lbError->setVisible(false);
    ui->lbError->setText("");
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->pbOk->setVisible(false);
    ui->pbRefresh->setVisible(true);

    m_bDownloading = false;

    bool check = false;
    check = connect(this, SIGNAL(sigDownLoadEnd(int)),
                    SLOT(slotDownLoadEnd(int)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigError(int,QString)),
                    SLOT(slotError(int,QString)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigProcess(double,double)),
                    SLOT(slotProcess(double,double)));
    Q_ASSERT(check);

#ifdef RABBITIM_USE_LIBCURL
    check = connect(this, SIGNAL(sigUpdateExec(int,QString)),
                    SLOT(slotUpdateExec(int,QString)));
    Q_ASSERT(check);
    //检查版本更新  
    on_pbRefresh_clicked();
#endif
    return ;
}

CDlgUpdate::~CDlgUpdate()
{
    delete ui;
}

#ifdef RABBITIM_USE_LIBCURL
void CDlgUpdate::slotUpdateExec(int nError, const QString &szFile)
{
    slotDownLoadVersionFile(nError, szFile);
    if(this->isHidden())
        exec();
}
#endif

void CDlgUpdate::slotDownLoadVersionFile(int nErrorCode, const QString &szFile)
{
    ui->lbPrompt->setText(tr("Be checking update version ..."));
    if(szFile.isEmpty())
    {
        slotError(-1, tr("The new version is not detected."));
        return ;
    }

    if(nErrorCode)
    {
        QString szErr = tr("Get version configure file fail:%1").arg(szFile);
        slotError(nErrorCode, szErr);
        return ;
    }

    QFile file(szFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open version configure file:%s", szFile.toStdString().c_str());
        QString szErr = tr("Don't open version configure file:%1").arg(szFile);
        slotError(-2, szErr);
        return ;
    }

    QString szErr;
    QDomDocument doc;
    if(!doc.setContent(&file, &szErr))
    {
        LOG_MODEL_ERROR("Update", "doc.setContent error:%s", szErr.toStdString().c_str());
        szErr = tr("version configure file format error.%1").arg(szFile);
        slotError(-3, szErr);
        return ;
    }

    if(doc.isNull())
    {
        LOG_MODEL_ERROR("Update", "doc is null");
        QString szErr = tr("version configure file format error.%1").arg(szFile);
        slotError(-4, szErr);
        return ;
    }

    QDomElement startElem = doc.documentElement();
    QString szMajorVersion = startElem.firstChildElement("MAJOR_VERSION_NUMBER").text();
    QString szMinorVersion = startElem.firstChildElement("MINOR_VERSION_NUMBER").text();
    QString szRevisionVersion = startElem.firstChildElement("REVISION_VERSION_NUMBER").text();
    /*这步已经在CDownloadHandle中判断  
    if(szMajorVersion.toInt() <= MAJOR_VERSION_NUMBER)
    {
        if(szMinorVersion.toInt() <= MINOR_VERSION_NUMBER)
        {
            if(szRevisionVersion.toInt() <= REVISION_VERSION_NUMBER)
            {
                LOG_MODEL_DEBUG("Update", "Is already the newest version.");
                this->accept();
                return;
            }
        }
    }*/

    m_szDownloadInfo = tr("New version:%1.%2.%3").arg(szMajorVersion, szMinorVersion, szRevisionVersion);
    QString szInfo = startElem.firstChildElement("INFO").text();
    if(!szInfo.isEmpty())
        m_szDownloadInfo += "\n" + szInfo;
    m_szDownLoadUrl = startElem.firstChildElement("URL").text();
    m_szDownloadMd5sum = startElem.firstChildElement("MD5SUM").text();
    if(m_szDownLoadUrl.isEmpty() || m_szDownloadMd5sum.isEmpty())
    {
        slotError(-5, tr("Version file is error. DownLoadUrl or md5sum is empty."));
        return;
    }

    CGlobal::Instance()->SetUpdateDate(QDateTime::currentDateTime());

    QUrl url(m_szDownLoadUrl);
    m_szDownLoadFile = CGlobalDir::Instance()->GetDirApplicationDownLoad()
            + QDir::separator() + url.fileName();
    QString szForce = startElem.firstChildElement("FORCE").text();
    if(szForce == "true")//强制更新  
    {
        slotDownLoadStart(false);
        return;
    }

    QString szMinCompatibleVersion = startElem.firstChildElement("MIN_COMPATIBLE_VERSION").text();
    if(!szMinCompatibleVersion.isNull())
    {
        QStringList lstNowVersion;
        lstNowVersion << QString::number(MAJOR_VERSION_NUMBER)
                      << QString::number(MINOR_VERSION_NUMBER)
                      << QString::number(REVISION_VERSION_NUMBER);
        QStringList lstVersion = szMinCompatibleVersion.split(".");
        int nSize = 3;
        if(lstVersion.size() < nSize)
            nSize = lstVersion.size();
        for(int i = 0; i < nSize; i++)
        {
            if(lstVersion[i] > lstNowVersion[i])
            {
                slotDownLoadStart(false);
                return;
            }
        }
    }

    slotDownLoadStart(true);
    return;
}

void CDlgUpdate::slotDownLoadStart(bool bPrompt)
{
    if(bPrompt)
    {
        QString szMsg = tr("Do you update new version?");
        if(!m_szDownloadInfo.isEmpty())
            szMsg += "\n" + m_szDownloadInfo;
        ui->lbPrompt->setText(szMsg);
        ui->progressBar->setVisible(false);
        ui->pbOk->setVisible(true);
        ui->pbRefresh->setVisible(true);
    }
    else
    {
        ui->pbOk->setVisible(false);
        DownloadFile();
    }

    ui->lbError->setVisible(true);
    ui->lbError->setText("");
}

void CDlgUpdate::slotError(int nErr, const QString &szErr)
{
    Q_UNUSED(nErr);
    ui->pbRefresh->setVisible(true);
    ui->lbError->setVisible(true);
    ui->lbError->setText(szErr);
}

void CDlgUpdate::slotProcess(double nTotal, double nNow)
{
    ui->progressBar->setValue(100 * nNow / nTotal);
}

int CDlgUpdate::DownloadFile()
{
    ui->lbPrompt->setText(tr("Downloading ") + m_szDownloadInfo);
    ui->progressBar->setVisible(true);
    ui->lbError->setText("");
    ui->lbError->setVisible(true);
    ui->pbOk->setVisible(false);
    ui->pbRefresh->setVisible(false);
    m_bDownloading = true;

    QFile f(m_szDownLoadFile);
    if(f.exists())
    {
        //检查文件是否正确，计算md5校验和  
        if(CTool::GetFileMd5SumString(m_szDownLoadFile) == m_szDownloadMd5sum)
        {
            ui->progressBar->setValue(100);
            slotDownLoadEnd(0);
            return 0;
        }
    }

    m_HandleDownLoad.SetFile(m_szDownLoadFile);
    m_HandleDownLoad.SetMd5sum(m_szDownloadMd5sum);
    return m_DownLoadFile.Start(m_szDownLoadUrl.toStdString(),
                                m_szDownLoadFile.toStdString(),
                                &m_HandleDownLoad, 10, 600);
}

void CDlgUpdate::slotDownLoadEnd(int nErr)
{
    ui->pbRefresh->setVisible(true);
    QString szMsg;
    if(nErr)
    {
        szMsg = tr("Download fail.");
        ui->lbPrompt->setText(szMsg);
        return;
    }

    szMsg = tr("Download success.") + m_szDownloadInfo;
    ui->lbPrompt->setText(szMsg);
    m_bDownloading = false;

    //检查文件是否正确，计算md5校验和  
    if(CTool::GetFileMd5SumString(m_szDownLoadFile) != m_szDownloadMd5sum)
    {
        LOG_MODEL_ERROR("Update", "md5sum is error.");
        QString szErr = tr("Download file md5sum error:%1").arg(m_szDownLoadFile);
        slotError(-1, szErr);
        return ;
    }

    //修改文件执行权限  
    /*QFileInfo info(m_szDownLoadFile);
    if(!info.permission(QFile::ExeUser))
    {
        //修改文件执行权限  
        QString szErr = tr("Download file don't execute permissions. Please modify permission then manually  execute it.\n%1").arg(m_szDownLoadFile);
        slotError(-2, szErr);
        return;
    }*/

    //启动安装程序  
    if(!m_Process.startDetached(m_szDownLoadFile))
    {
        QUrl url(m_szDownLoadFile);
        if(!QDesktopServices::openUrl(url))
        {
            LOG_MODEL_ERROR("Update", "open proess error:%s", m_szDownLoadFile.toStdString().c_str());
            QString szErr = tr("Execute install program error.%1").arg(m_szDownLoadFile);
            slotError(-2, szErr);
            return;
        }
    }
    //关闭程序  
    MainWindow* pMain = (MainWindow*)GET_MAINWINDOW;
    pMain->close();
}

void CDlgUpdate::on_pbOk_clicked()
{
    DownloadFile();
}

void CDlgUpdate::on_pbCancel_clicked()
{
    if(m_bDownloading)
    {
        m_DownLoadFile.Exit();
        m_bDownloading = false;
        ui->pbOk->setVisible(true);
        return;
    }
    this->reject();
}

void CDlgUpdate::changeEvent(QEvent *e)
{
    //LOG_MODEL_DEBUG("MainWindow", "MainWindow::changeEvent.e->type:%d", e->type());
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->lbCurrentVersion->setText(tr("Current version:")
                                      + QString::number(MAJOR_VERSION_NUMBER)
                                      + "."
                                      + QString::number(MINOR_VERSION_NUMBER)
                                      + "."
                                      + QString::number(REVISION_VERSION_NUMBER));
        ui->lbPrompt->setText(tr("The new version is not detected."));
        break;
    }
}

void CDlgUpdate::showEvent(QShowEvent *)
{
}

void CDlgUpdate::on_pbRefresh_clicked()
{
#ifdef RABBITIM_USE_LIBCURL
    //检查版本更新  
    m_Update.Start();
#endif
}
