#include "FrmRegister.h"
#include "ui_FrmRegister.h"
#include "../FrmLogin/FrmLogin.h"
#include "../../MainWindow.h"
#include "Global/Global.h"
#include <QDesktopWidget>

CFrmRegister::CFrmRegister(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmRegister)
{
    ui->setupUi(this);

    bool check = connect(GET_CLIENT.data(), SIGNAL(sigClientConnected()),
                         SLOT(connected()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigClientError(CClient::ERROR_TYPE)),
                    SLOT(clientError(CClient::ERROR_TYPE)));

    QDesktopWidget *pDesk = QApplication::desktop();    
    move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);
}

CFrmRegister::~CFrmRegister()
{
    LOG_MODEL_DEBUG("Register",  "CFrmRegister::~CFrmRegister");

    delete ui;
}
/*
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

    GET_CLIENT->Logout();
}*/

void CFrmRegister::clientError(CClient::ERROR_TYPE e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Register", "CFrmRegister::clientError");
}

void CFrmRegister::connected()
{
    GET_CLIENT->Register(ui->txtUser->text(), ui->txtPassword->text(), ui->txtemail->text(), ui->txtInstructions->text());
}

void CFrmRegister::on_pbCreate_clicked()
{
    if(ui->txtUser->text().isEmpty()
            || ui->txtUser->text().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill user name, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(ui->txtPassword->text().isEmpty()
            || ui->txtPassword->text().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill password, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(ui->txtPassword->text() != ui->txtConfirmPassword->text())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Confirm password error, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(ui->txtPassword->text().indexOf(QRegExp("[^a-zA-Z0-9\`\~\!\@\#\$\%\^\&\*\(\)]")) != -1)
    {
        QMessageBox::critical(this, tr("Register error"), 
                              tr("Password contains illegal characters. Only letters, numbers, `~!@#$%^&*() combinations"));
        return;
    }
    if(ui->txtemail->text().isEmpty()
            || ui->txtemail->text().isNull())
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill email, Please refilling"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(ui->txtemail->text().indexOf("@") == -1)
    {
        QMessageBox::critical(this, tr("Register error"),
                              tr("Email format is wrong"));
        return;
    }

    GET_CLIENT->Login();
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

void CFrmRegister::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}
