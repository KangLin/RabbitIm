#include "DlgUpdate.h"
#include "ui_DlgUpdate.h"
#include "Tool.h"
#include "Version.h"
#include "Global/Global.h"
#include <QDir>
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

    bool check = connect(this, SIGNAL(sigDownLoadStart(bool)),
                         SLOT(slotDownLoadStart(bool)));
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

void CDlgUpdate::slotDownLoadStart(bool bPrompt)
{
    if(bPrompt)
    {
        QString szMsg = tr("Do you update new version?");
        if(!m_VersionFileHandle.m_szInfo.isEmpty())
            szMsg += "\n" + m_VersionFileHandle.m_szInfo;
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
    }
    else
        szMsg = tr("Download success.") + m_VersionFileHandle.m_szInfo;
    ui->lbPrompt->setText(szMsg);
    m_bDownloading = false;
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
    ui->lbPrompt->setText(tr("Downloading ") + m_VersionFileHandle.m_szInfo);
    ui->progressBar->setVisible(true);
    ui->lbError->setText("");
    ui->lbError->setVisible(true);
    ui->pbOk->setEnabled(false);
    m_bDownloading = true;

    m_HandleDownLoad.SetFile(m_VersionFileHandle.m_szDownLoadFile);
    m_HandleDownLoad.SetMd5sum(m_VersionFileHandle.m_szMd5sum);
    return m_DownLoadFile.Start(m_VersionFileHandle.m_szUrl.toStdString(),
                                m_VersionFileHandle.m_szDownLoadFile.toStdString(), &m_HandleDownLoad, 10, 600);
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
