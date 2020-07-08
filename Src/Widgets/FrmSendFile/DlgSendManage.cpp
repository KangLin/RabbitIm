#include "DlgSendManage.h"
#include "ui_DlgSendManage.h"
#include "FrmSendFile.h"

CDlgSendManage::CDlgSendManage(QWidget *parent) :
    QDialog(parent),
    m_SendCount(0),
    ui(new Ui::CDlgSendManage)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::WindowMinMaxButtonsHint);
    this->setWindowTitle(tr("send file"));
}

CDlgSendManage::~CDlgSendManage()
{
    delete ui;
}

void CDlgSendManage::addFileProcess(QXmppTransferJob &job, bool isSend)
{
    CFrmSendFile* bar = new CFrmSendFile(job,isSend);
    connect(bar,SIGNAL(sigClosed()),this,SLOT(onSigClosed()));
    ui->vLayout->insertWidget(0,bar);
    this->show();
    m_SendCount++;
}

void CDlgSendManage::onSigClosed()
{
    m_SendCount--;
    if(m_SendCount <= 0)
    {
        m_SendCount = 0;
        this->hide();
    }
}
