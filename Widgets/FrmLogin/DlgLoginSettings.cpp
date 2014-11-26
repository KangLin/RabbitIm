#include "DlgLoginSettings.h"
#include "ui_DlgLoginSettings.h"
#include "../../Global/Global.h"
#include <QDesktopWidget>

CDlgLoginSettings::CDlgLoginSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgLoginSettings)
{
    LOG_MODEL_DEBUG("Login", "CFrmLoginSettings::CFrmLoginSettings");
    ui->setupUi(this);

    ui->txtXmppDomain->setText(CGlobal::Instance()->GetXmppDomain());
    ui->txtXmppServer->setText(CGlobal::Instance()->GetXmppServer());
    ui->txtXmppPort->setText(QString::number(CGlobal::Instance()->GetXmppServerPort()));
    ui->txtStunServer->setText(CGlobal::Instance()->GetStunServer());
    ui->txtStunPort->setText(QString::number(CGlobal::Instance()->GetStunServerPort()));
    ui->txtTurnServer->setText(CGlobal::Instance()->GetTurnServer());
    ui->txtTurnPort->setText(QString::number(CGlobal::Instance()->GetTurnServerPort()));
    ui->txtUser->setText(CGlobal::Instance()->GetTurnServerUser());
    ui->txtPassword->setText(CGlobal::Instance()->GetTurnServerPassword());

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
}

CDlgLoginSettings::~CDlgLoginSettings()
{
    delete ui;
    LOG_MODEL_DEBUG("Login", "CFrmLoginSettings::~CFrmLoginSettings");
}

void CDlgLoginSettings::closeEvent(QCloseEvent *)
{
}

void CDlgLoginSettings::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

void CDlgLoginSettings::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgLoginSettings::on_pbOK_clicked()
{
    CGlobal::Instance()->SetXmppDomain(ui->txtXmppDomain->text());
    CGlobal::Instance()->SetXmppServer(ui->txtXmppServer->text());
    CGlobal::Instance()->SetXmppServerPort(ui->txtXmppPort->text().toInt());
    CGlobal::Instance()->SetStunServer(ui->txtStunServer->text());
    CGlobal::Instance()->SetStunServerPort(ui->txtStunPort->text().toInt());
    CGlobal::Instance()->SetTurnServer(ui->txtTurnServer->text());
    CGlobal::Instance()->SetTurnServerPort(ui->txtTurnPort->text().toInt());
    CGlobal::Instance()->SetTurnServerUser(ui->txtUser->text());
    CGlobal::Instance()->SetTurnServerPassword(ui->txtPassword->text());

    close();
}

void CDlgLoginSettings::on_pbCanecl_clicked()
{
    close();
}
