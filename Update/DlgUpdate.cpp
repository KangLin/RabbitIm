#include "DlgUpdate.h"
#include "ui_DlgUpdate.h"
#include "Tool.h"
#include "Version.h"
#include "Global/Global.h"
#include <QDir>
#include <QtXml>
#include "MainWindow.h"

CDlgUpdate::CDlgUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUpdate), 
    m_VersionFileHandle(std::string(), this),
    m_HandleDownLoad(this)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);

    ui->lbPrompt->setText(tr("Is checking update version"));
    ui->lbError->setVisible(false);
    ui->lbError->setText("");
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->pbOk->setEnabled(false);

    m_bDownloading = false;

    bool check = false;
    check = connect(this, SIGNAL(sigDownLoadVersionFile(int)),
                    SLOT(slotDownLoadVersionFile(int)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigDownLoadEnd(int)),
                    SLOT(slotDownLoadEnd(int)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigError(int,QString)),
                    SLOT(slotError(int,QString)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigProcess(double,double)),
                    SLOT(slotProcess(double,double)));
    Q_ASSERT(check);

    return ;
}

CDlgUpdate::~CDlgUpdate()
{
    delete ui;
}

int CDlgUpdate::Start()
{
    QDateTime d = CGlobal::Instance()->GetUpdateDate();
    int nDays = d.daysTo(QDateTime::currentDateTime());
    CGlobal::E_UPDATE updateType = CGlobal::Instance()->GetUpdate();
    switch(updateType)
    {
    case CGlobal::E_UPDATE_EVERY_TIME:
        break;
    case CGlobal::E_UPDATE_DAY:
        if(nDays < 1)
            return 0;
        break;
    case CGlobal::E_UPDATE_WEEK:
        if(nDays < 7)
            return 0;
        break;
    case CGlobal::E_UPDATE_MONTH:
        if(nDays < 30)
            return 0;
        break;
    default:
        return 0;
    }

    QString szFile = "Update_";
    szFile += RABBITIM_SYSTEM;
    /*szFile += "_";
    szFile += RABBITIM_PLATFORM;
    szFile += "_";
    szFile += RABBITIM_ARCHITECTURE;*/
    szFile += ".xml";
    m_szVersionFile = CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + szFile;
    m_szUrl = "https://code.csdn.net/kl222/rabbitim/blob/Reconstruction/Update/" + szFile;
    m_VersionFileHandle.SetFile(m_szVersionFile.toStdString());
    m_DownLoadVersion.Start(m_szUrl.toStdString(), 
                     m_szVersionFile.toStdString(), 
                     &m_VersionFileHandle, 
                     1);

    CGlobal::Instance()->SetUpdateDate(QDateTime::currentDateTime());
    return 0;
}

void CDlgUpdate::slotDownLoadVersionFile(int nErrorCode)
{
    if(m_szVersionFile.isEmpty())
        return ;

    if(nErrorCode)
    {
        QString szErr = tr("Download file fail:%1").arg(m_szVersionFile);
        slotError(nErrorCode, szErr);
        return ;
    }

    QFile file(m_szVersionFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open file:%s", m_szVersionFile.toStdString().c_str());
        QString szErr = tr("Don't open file:%1").arg(m_szVersionFile);
        slotError(-2, szErr);
        return ;
    }

    QString szErr;
    QDomDocument doc;
    if(!doc.setContent(&file, &szErr))
    {
        LOG_MODEL_ERROR("Update", "doc.setContent error:%s", szErr.toStdString().c_str());
        szErr = tr("File format error.%1").arg(m_szVersionFile);
        slotError(-3, szErr);
        return ;
    }

    if(doc.isNull())
    {
        LOG_MODEL_ERROR("Update", "doc is null");
        QString szErr = tr("File format error.%1").arg(m_szVersionFile);
        slotError(-4, szErr);
        return ;
    }

    QDomElement startElem = doc.documentElement();
    QString szMajorVersion = startElem.firstChildElement("MAJOR_VERSION_NUMBER").text();
    QString szMinorVersion = startElem.firstChildElement("MINOR_VERSION_NUMBER").text();
    QString szRevisionVersion = startElem.firstChildElement("REVISION_VERSION_NUMBER").text();
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
    }

    m_szDownloadInfo = tr("New version:%1.%2.%3").arg(szMajorVersion, szMinorVersion, szRevisionVersion);
    m_szDownloadInfo += "\n" + startElem.firstChildElement("INFO").text();
    m_szDownLoadUrl = startElem.firstChildElement("URL").text();
    m_szDownloadMd5sum = startElem.firstChildElement("MD5SUM").text();
    if(m_szDownLoadUrl.isEmpty() || m_szDownloadMd5sum.isEmpty())
    {
        slotError(-5, tr("Version file is error"));
        return;
    }
    QUrl url(m_szDownLoadUrl);
    m_szDownLoadFile= CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + url.fileName();
    QString szForce = startElem.firstChildElement("FORCE").text();
    if(szForce == "true")//强制更新  
    {
        slotDownLoadStart(false);
        return;
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
        ui->pbOk->setEnabled(true);
    }
    else
    {
        ui->pbOk->setEnabled(false);
        DownloadFile();
    }

    ui->lbError->setVisible(true);
    ui->lbError->setText("");
    emit GET_MAINWINDOW->sigUpdateExec();
}

void CDlgUpdate::slotDownLoadEnd(int nErr)
{
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
    QFileInfo info(m_szDownLoadFile);
    if(!info.permission(QFile::ExeUser))
    {
        //修改文件执行权限  
        QString szErr = tr("Download file don't execute permissions. Please modify permission then manually  execute it.\n%1").arg(m_szDownLoadFile);
        slotError(-2, szErr);
        return;
    }

    //启动安装程序  
    if(!m_Process.startDetached(m_szDownLoadFile))
    {
        LOG_MODEL_ERROR("Update", "open proess error:%s", m_szDownLoadFile.toStdString().c_str());
        QString szErr = tr("Execute install program error.%1").arg(m_szDownLoadFile);
        slotError(-2, szErr);
        return;
    }
    //关闭程序  
    MainWindow* pMain = (MainWindow*)GET_MAINWINDOW;
    pMain->close();
}

void CDlgUpdate::slotError(int nErr, const QString &szErr)
{
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
    ui->pbOk->setEnabled(false);
    m_bDownloading = true;

    m_HandleDownLoad.SetFile(m_szDownLoadFile);
    m_HandleDownLoad.SetMd5sum(m_szDownloadMd5sum);
    return m_DownLoadFile.Start(m_szDownLoadUrl.toStdString(),
                                m_szDownLoadFile.toStdString(), &m_HandleDownLoad, 10, 600);
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
        ui->pbOk->setEnabled(true);
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
        break;
    }
}
