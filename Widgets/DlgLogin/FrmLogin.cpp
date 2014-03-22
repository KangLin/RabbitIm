#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global.h"
#include "../FrmRegister/DlgRegister.h"

extern CGlobal g_Global;

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLogin)
{
    ui->setupUi(this);
    m_pRegister = new CFrmRegister();

    //TODO:发行时删除下面行---------------
    ui->lnServer->setText("183.62.225.76");
    ui->lnServer->setVisible(false);
    ui->lnUser->setText("a");
    ui->lnPassword->setText("a");
    ui->lbePrompt->setText("");
    //---------------------------------
}

CFrmLogin::~CFrmLogin()
{
    delete ui;

    if(m_pRegister)
        delete m_pRegister;
}

void CFrmLogin::on_pbOk_clicked()
{
    bool check = connect(((MainWindow*)(this->parent()))->m_pClient,
                         SIGNAL(connected()),
                         (MainWindow*)(this->parent()),
                         SLOT(clientConnected()));
    Q_ASSERT(check);

    ui->lbePrompt->setText("");

    QXmppConfiguration config;
    //TODO:设置为非sasl验证
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

void CFrmLogin::on_pbRegitster_clicked()
{
    if(m_pRegister)
    {
        this->setEnabled(false);
        m_pRegister->SetLogin(this);
        m_pRegister->show();
        m_pRegister->activateWindow();
    }
}

int CFrmLogin::SetPrompt(QString szPrompt)
{
    int nRet = 0;
    ui->lbePrompt->setText(szPrompt);
    return nRet;
}

int CFrmLogin::SetLoginInformation(QString szName, QString szPassword)
{
    ui->lnUser->setText(szName);
    ui->lnPassword->setText(szPassword);
    return 0;
}
