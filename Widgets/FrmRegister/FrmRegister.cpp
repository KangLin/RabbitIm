#include "FrmRegister.h"
#include "ui_FrmRegister.h"
#include "../DlgLogin/FrmLogin.h"
#include "qxmpp/QXmppRegisterIq.h"
#include "../../MainWindow.h"
#include "../../Global.h"

extern CGlobal g_Global;

CFrmRegister::CFrmRegister(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmRegister)
{
    ui->setupUi(this);

    m_pClient = new CXmppClient(this);
    if(NULL == m_pClient)
    {
        LOG_MODEL_ERROR("Register", "CFrmRegister::CFrmRegister: create CXmppClient fail");
        return;
    }

    bool check = connect(m_pClient, SIGNAL(connected()),
                         SLOT(connected()));
    Q_ASSERT(check);
    check = connect(m_pClient, SIGNAL(error(QXmppClient::Error)),
                    SLOT(clientError(QXmppClient::Error)));
    Q_ASSERT(check);

    check = connect(m_pClient, SIGNAL(iqReceived(QXmppIq)),
                    SLOT(clientIqReceived(QXmppIq)));
    Q_ASSERT(check);
}

CFrmRegister::~CFrmRegister()
{
     LOG_MODEL_DEBUG("Register",  "CFrmRegister::~CFrmRegister");

    if(m_pClient)
        delete m_pClient;

    delete ui;
}

void CFrmRegister::clientIqReceived(const QXmppIq &iq)
{
    LOG_MODEL_DEBUG("Register", "CFrmRegister::clientIqReceived");
    if(iq.type() == QXmppIq::Result)
    {
        ((CFrmLogin*)m_pLogin)->SetLoginInformation(ui->txtUser->text(), ui->txtPassword->text());
        ((CFrmLogin*)m_pLogin)->SetPrompt(tr("Register success"));

        if(m_pLogin)
            m_pLogin->setEnabled(true);

        close();
    }
    else if(iq.type() == QXmppIq::Error)
    {
        LOG_MODEL_DEBUG("Register", "CFrmRegister::clientIqReceived:%d", iq.error().code());
        QString szReason(tr("Unknow error:") + iq.error().code());
        if(iq.error().condition() == QXmppIq::Error::Conflict)
        {
            szReason = tr("User") + " [" + ui->txtUser->text() + "] " + tr("had exist");
        }
        else if(iq.error().condition() == QXmppIq::Error::InternalServerError)
        {
            szReason = tr("Sever internal error");
        }

        QMessageBox msg(QMessageBox::Critical,
                        tr("Register fail"),
                        szReason,
                        QMessageBox::Ok);
        msg.exec();
    }

    m_pClient->disconnectFromServer();
}

void CFrmRegister::clientError(QXmppClient::Error e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Register", "CFrmRegister::clientError");
}

void CFrmRegister::connected()
{
    LOG_MODEL_DEBUG("Register", "CFrmRegister::connected");

    QXmppRegisterIq registerIq;
    registerIq.setType(QXmppIq::Set);
    registerIq.setUsername(ui->txtUser->text());
    if(registerIq.username().isEmpty()
            || registerIq.username().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill user name, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setPassword(ui->txtPassword->text());
    if(registerIq.password().isEmpty()
            || registerIq.password().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill password, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(registerIq.password() != ui->txtConfirmPassword->text())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Confirm password error, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setEmail(ui->txtemail->text());
    if(registerIq.email().isEmpty()
            || registerIq.email().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill email, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setInstructions(ui->txtInstructions->text());

    m_pClient->sendPacket(registerIq);
}

void CFrmRegister::on_pbCreate_clicked()
{
    QXmppRegisterIq registerIq;

    registerIq.setUsername(ui->txtUser->text());
    if(registerIq.username().isEmpty()
            || registerIq.username().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill user name, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setPassword(ui->txtPassword->text());
    if(registerIq.password().isEmpty()
            || registerIq.password().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill password, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(registerIq.password() != ui->txtConfirmPassword->text())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Confirm password error, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setEmail(ui->txtemail->text());
    if(registerIq.email().isEmpty()
            || registerIq.email().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill email, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    registerIq.setInstructions(ui->txtInstructions->text());

    QXmppConfiguration config;
    //TODO:设置为非sasl验证和服务器IP
    config.setUseSASLAuthentication(false);
    config.setUseNonSASLAuthentication(false);

    config.setHost(g_Global.GetXmppServerHost());
    config.setDomain(g_Global.GetXmppServer());
    m_pClient->connectToServer(config);
}

int CFrmRegister::SetLogin(QWidget *pLogin)
{
    m_pLogin = pLogin;
    return 0;
}

void CFrmRegister::on_pbCancel_clicked()
{
    if(m_pLogin)
        m_pLogin->setEnabled(true);

    close();
}

void CFrmRegister::hideEvent(QHideEvent *)
{
    LOG_MODEL_DEBUG("Register", "CFrmRegister::hideEvent");
    if(m_pLogin)
        m_pLogin->setEnabled(true);

    close();
}
