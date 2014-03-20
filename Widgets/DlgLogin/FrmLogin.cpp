#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global.h"

extern CGlobal g_Global;

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLogin)
{
    ui->setupUi(this);
    ui->lnServer->setText("183.62.225.76");
    ui->lnUser->setText("admin");
    ui->lnPassword->setText("rdc123456");
    ui->lbePrompt->setText("");
}

CFrmLogin::~CFrmLogin()
{
    delete ui;
}

void CFrmLogin::on_pbOk_clicked()
{
    ui->lbePrompt->setText("");
    QXmppConfiguration config;
    config.setUseSASLAuthentication(false);
    config.setHost(ui->lnServer->text());
    config.setUser(ui->lnUser->text());
    config.setPassword(ui->lnPassword->text());
    g_Global.SetJid(config.jid());
    qDebug("Local jid:%s", qPrintable(g_Global.GetBareJid()));
    ((MainWindow*)(this->parent()))->m_pClient->connectToServer(config);
}

void CFrmLogin::on_pbClose_clicked()
{
    //退出程序
    ((QWidget*)this->parent())->close();
}

int CFrmLogin::SetPrompt(QString szPrompt)
{
    int nRet = 0;
    ui->lbePrompt->setText(szPrompt);
    return nRet;
}
