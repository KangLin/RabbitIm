#include "DlgUpdate.h"
#include "ui_DlgUpdate.h"
#include "Tool.h"
#include "Version.h"
#include "Global/Global.h"
#include <QDir>

CDlgUpdate::CDlgUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUpdate), 
    m_VersionFileHandle(std::string(), this),
    m_HandleDownLoad(this)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);

    ui->lbPrompt->setText("");
    ui->lbError->setVisible(false);
    ui->lbError->setText("");
    ui->progressBar->setVisible(false);
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

    QString szFile = "Update_";
    szFile += RABBITIM_SYSTEM;
    /*szFile += "_";
    szFile += RABBITIM_PLATFORM;
    szFile += "_";
    szFile += RABBITIM_ARCHITECTURE;*/
    szFile += ".xml";
    m_szVersionFile = CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + szFile;
    m_szUrl = "https://code.csdn.net/kl222/rabbitim/blob/Reconstruction/Update/" + szFile;
    //m_szUrl = "http://182.254.185.29/a";
    m_VersionFileHandle.SetFile(m_szVersionFile.toStdString());
    m_DownLoadVersion.Start(m_szUrl.toStdString(), 
                     m_szVersionFile.toStdString(), 
                     &m_VersionFileHandle, 
                     1);

    return ;
}

CDlgUpdate::~CDlgUpdate()
{
    delete ui;
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
    ui->lbPrompt->setText(tr("Downloading: ") + m_VersionFileHandle.m_szInfo);
    ui->progressBar->setVisible(true);
    ui->lbError->setText("");
    ui->lbError->setVisible(true);
    ui->pbOk->setEnabled(false);
    m_bDownloading = true;

    m_HandleDownLoad.SetFile(m_VersionFileHandle.m_szDownLoadFile);
    m_HandleDownLoad.SetMd5sum(m_VersionFileHandle.m_szMd5sum);
    return m_DownLoadFile.Start(m_VersionFileHandle.m_szUrl.toStdString(),
                                m_VersionFileHandle.m_szDownLoadFile.toStdString(), &m_HandleDownLoad, 10, 60);
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
