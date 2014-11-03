#include "DlgRegister.h"
#include "ui_DlgRegister.h"
#include "../FrmLogin/FrmLogin.h"
#include "../../MainWindow.h"
#include "Global/Global.h"
#include <QDesktopWidget>

CDlgRegister::CDlgRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgRegister)
{
    ui->setupUi(this);

    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->availableGeometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif
}

CDlgRegister::~CDlgRegister()
{
    delete ui;
}

void CDlgRegister::clientError(CClient::ERROR_TYPE e)
{
    if(CClient::NoError == e)
    {
        ((CFrmLogin*)m_pLogin)->SetLoginInformation(ui->txtUser->text(), ui->txtPassword->text());
        ((CFrmLogin*)m_pLogin)->SetPrompt(tr("Register success"));

        if(m_pLogin)
            m_pLogin->setEnabled(true);

        close();
    }
    else
    {
        QString szReason(tr("Unknow error"));
        if(CClient::Conflict == e)
        {
            szReason = tr("User") + " [" + ui->txtUser->text() + "] " + tr("had exist");
        }
        else if(CClient::InternalServerError == e)
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
}

void CDlgRegister::connected()
{
    GET_CLIENT->Register(ui->txtUser->text(), ui->txtPassword->text(), ui->txtemail->text(), ui->txtInstructions->text());
}

void CDlgRegister::on_pbCreate_clicked()
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

    GET_CLIENT.data()->disconnect(this);
    bool check = connect(GET_CLIENT.data(), SIGNAL(sigClientConnected()),
                         SLOT(connected()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigClientError(CClient::ERROR_TYPE)),
                    SLOT(clientError(CClient::ERROR_TYPE)));
    
    GET_CLIENT->Login();
}

int CDlgRegister::SetLogin(QWidget *pLogin)
{
    m_pLogin = pLogin;
    return 0;
}

void CDlgRegister::on_pbCancel_clicked()
{
    close();
}

void CDlgRegister::hideEvent(QHideEvent *)
{
    LOG_MODEL_DEBUG("Register", "CFrmRegister::hideEvent");
    if(m_pLogin)
        m_pLogin->setEnabled(true);

    close();
}

void CDlgRegister::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}
