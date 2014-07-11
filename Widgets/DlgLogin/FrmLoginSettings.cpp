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

    ui->txtXmppServer->setText(g_Global.GetXmppServerHost());
    ui->txtXmppPort->setText(QString::number(g_Global.GetXmppServerPort()));
    ui->txtStunServer->setText(g_Global.GetStunServer());
    ui->txtStunPort->setText(QString::number(g_Global.GetStunServerPort()));
    ui->txtTurnServer->setText(g_Global.GetTurnServer());
    ui->txtTurnPort->setText(QString::number(g_Global.GetTurnServerPort()));
    ui->txtUser->setText(g_Global.GetTurnServerUser());
    ui->txtPassword->setText(g_Global.GetTurnServerPassword());

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
    g_Global.SetXmppServerHost(ui->txtXmppServer->text());
    g_Global.SetXmppServerPort(ui->txtXmppPort->text().toInt());
    g_Global.SetStunServer(ui->txtStunServer->text());
    g_Global.SetStunServerPort(ui->txtStunPort->text().toInt());
    g_Global.SetTurnServer(ui->txtTurnServer->text());
    g_Global.SetTurnServerPort(ui->txtTurnPort->text().toInt());
    g_Global.SetTurnServerUser(ui->txtUser->text());
    g_Global.SetTurnServerPassword(ui->txtPassword->text());

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
