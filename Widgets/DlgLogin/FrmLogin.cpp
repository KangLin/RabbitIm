#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global.h"

extern CGlobal g_Global;

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLogin)
{
    ui->setupUi(this);
    m_pRegister = new CFrmRegister();

    //TODO:发行时删除下面行---------------
    ui->lnServer->setText(g_Global.GetXmppServerHost());
    //ui->lnServer->setVisible(false);
#ifdef ANDROID
    ui->lnUser->setText("a");
    ui->lnPassword->setText("a");
#else
    ui->lnUser->setText("b");
    ui->lnPassword->setText("b");
#endif
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

    ui->lbePrompt->setText(tr("Being Login..."));

    QXmppConfiguration config;
    //TODO:设置为非sasl验证
    config.setUseSASLAuthentication(false);
    //config.setUseNonSASLAuthentication(false);
    config.setHost(ui->lnServer->text());
    config.setPort(g_Global.GetXmppServerPort());
    config.setDomain(g_Global.GetXmppServer());
    config.setUser(ui->lnUser->text());
    config.setPassword(ui->lnPassword->text());
    g_Global.SetJid(config.jid());
    g_Global.SetXmppServer(ui->lnServer->text());

    qDebug("Local jid:%s;config.jidBare():%s",
           qPrintable(g_Global.GetBareJid()),
           qPrintable(config.jidBare()));
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
