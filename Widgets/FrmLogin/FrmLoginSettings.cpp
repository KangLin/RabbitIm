#include "FrmLoginSettings.h"
#include "ui_FrmLoginSettings.h"
#include "../../Global.h"
#include <QDesktopWidget>

CFrmLoginSettings::CFrmLoginSettings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLoginSettings)
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

    m_pParent = parent;
}

CFrmLoginSettings::~CFrmLoginSettings()
{
    delete ui;
    LOG_MODEL_DEBUG("Login", "CFrmLoginSettings::~CFrmLoginSettings");
}

void CFrmLoginSettings::closeEvent(QCloseEvent *)
{
    if(m_pParent)
        m_pParent->setEnabled(true);

    deleteLater();
}

void CFrmLoginSettings::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if(m_pParent)
        m_pParent->setEnabled(false);
}

int CFrmLoginSettings::SetLogin(QWidget *pLogin)
{
    m_pParent = pLogin;
    return 0;
}

void CFrmLoginSettings::on_pbOK_clicked()
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

    if(m_pParent)
        m_pParent->setEnabled(true);
    close();
}

void CFrmLoginSettings::on_pbCanecl_clicked()
{
    if(m_pParent)
        m_pParent->setEnabled(true);
    close();
}
