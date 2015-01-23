#include "DlgAddRoster.h"
#include "ui_DlgAddRoster.h"
#include "../../Global/Global.h"
#include <string>
#include <QMessageBox>
#include <QDesktopWidget>
#include "Tool.h"

CDlgAddRoster::CDlgAddRoster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgAddRoster)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    ui->txtGroup->setEditable(true);

    m_bRequest = false;
}

CDlgAddRoster::~CDlgAddRoster()
{
    delete ui;
}

int CDlgAddRoster::Init(QSet<QString> groups, QString szId, bool bRequest)
{
    ui->txtJID->clear();
    ui->txtNick->clear();
    ui->txtGroup->clear();
    m_bRequest = bRequest;
    if(bRequest)
    {
        this->setWindowTitle(tr("Request add roster"));
        ui->lbPrompt->setText(tr("%1 request add roster").arg(szId));
        ui->txtJID->setEnabled(false);
    }
    else
    {
        this->setWindowTitle(tr("Add Roster"));
        ui->lbPrompt->setText(tr("Add Roster"));
        ui->txtJID->setEnabled(true);
    }
    ui->txtJID->setText(szId);
    on_txtJID_editingFinished();
    if(groups.isEmpty())
    {
        ui->txtGroup->setCurrentText(tr("My friends"));
    }
    else
    {
        foreach(QString g, groups)
        {
            ui->txtGroup->addItem(g);
        }
    }
    return 0;
}

void CDlgAddRoster::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgAddRoster::on_pbOk_clicked()
{
    QString szJid = ui->txtJID->text();
    if(szJid.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Isn't fill JID. Please fill it."));
        return;
    }
    QString szNick = ui->txtNick->text();
    if(szNick.isEmpty())
        szNick = szJid.left(szJid.indexOf("@"));
    QSet<QString> groups;
    groups << ui->txtGroup->currentText();
    CClient::SUBSCRIBE_TYPE type;
    if(m_bRequest)
        type = CClient::SUBSCRIBE_ACCEPT;
    else
        type = CClient::SUBSCRIBE_REQUEST;
    GET_CLIENT->RosterAdd(szJid, type, ui->txtNick->text(), groups);
    this->accept();
}

void CDlgAddRoster::on_pbCancel_clicked()
{
    if(m_bRequest)
        GET_CLIENT->RosterAdd(ui->txtJID->text(), CClient::SUBSCRIBE_REFUSE);
    this->reject();
}

void CDlgAddRoster::on_txtJID_editingFinished()
{
    LOG_MODEL_DEBUG("Roster", "CFrmAddRoster::on_txtJID_editingFinished");
    if(ui->txtNick->text().isEmpty())
    {
        QString szJid = ui->txtJID->text();
        int nPos = szJid.indexOf("@");
        if(nPos > 0)
        {
            //检查是否已经存在  
            QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szJid);
            if(user.isNull())
                ui->txtNick->setText(szJid.left(nPos));
            else
                ui->txtNick->setText(user->GetInfo()->GetName());
        }
        else
        {
            ui->txtNick->setText(szJid);
        }
    }
}
