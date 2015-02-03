#include "DlgRegister.h"
#include "ui_DlgRegister.h"
#include "../FrmLogin/FrmLogin.h"
#include "../../MainWindow.h"
#include "Global/Global.h"
#include "Tool.h"

CDlgRegister::CDlgRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgRegister)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    m_pLogin = parent;
}

CDlgRegister::~CDlgRegister()
{
    GET_CLIENT->disconnect(this);
    delete ui;
}

void CDlgRegister::clientError(CClient::ERROR_TYPE e)
{
    ui->pbCreate->setEnabled(true);
    GET_CLIENT->disconnect(this);
    if(CClient::NoError == e)
    {
        ((CFrmLogin*)m_pLogin)->SetLoginInformation(ui->txtId->text(), ui->txtPassword->text());
        ((CFrmLogin*)m_pLogin)->SetPrompt(tr("Register success"));
        this->accept();
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
    QSharedPointer<CUserInfo> user = GETMANAGER->GetManageUser()->NewUser()->GetInfo();
    user->SetId(ui->txtId->text());
    user->SetName(ui->txtUser->text());
    user->SetEmail(ui->txtemail->text());
    user->SetDescription(ui->txtInstructions->text());
    user->SetUrl(ui->txtUrl->text());
    user->SetBirthday(ui->dtBirthday->date());
    user->SetPhoto(m_PhotoBuffer.buffer());
    GET_CLIENT->Register(user, ui->txtPassword->text());
}

void CDlgRegister::on_pbCreate_clicked()
{
    if(ui->txtId->text().isEmpty()
            || ui->txtId->text().isNull() 
            || ui->txtId->text().length() < 4 || ui->txtId->text().length() > 8)
    {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Register error"),
                        tr("Don't fill user id, Please refilling, the id length must more than 4 and less then 8"),
                        QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(ui->txtId->text().indexOf(QRegExp("[^a-zA-Z0-9]")) != -1)
    {
        QMessageBox::critical(this, tr("Register error"), 
                              tr("User id contains illegal characters. Only letters, numbers, combinations"));
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
    if(ui->txtPassword->text().indexOf(QRegExp("[^a-zA-Z0-9`~!@#$%^&*()]")) != -1)
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

    GET_CLIENT->disconnect(this);
    bool check = connect(GET_CLIENT.data(), SIGNAL(sigClientConnected()),
                         SLOT(connected()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigClientError(CClient::ERROR_TYPE)),
                    SLOT(clientError(CClient::ERROR_TYPE)));

    GET_CLIENT->Login();
    ui->pbCreate->setEnabled(false);
}

void CDlgRegister::on_pbCancel_clicked()
{
    this->reject();
}

void CDlgRegister::hideEvent(QHideEvent *)
{
    LOG_MODEL_DEBUG("Register", "CFrmRegister::hideEvent");
}

void CDlgRegister::changeEvent(QEvent *e)
{
    switch((int)e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgRegister::on_pbBrower_clicked()
{
    QString szFile, szFilter("*.png *.jpg *.bmp *.gif *.jpeg");
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(szFile.isEmpty())
       return; 

    //TODO:现在只上传小图片,以后增加上传原图  
    //原因是openfire把vcard存在数据库中,导制数据库存性能,网络性能降低  
    QPixmap pixmap(szFile), map;
    int nWidth = pixmap.width();
    int nHeight = pixmap.height();
    if(nWidth > RABBITIM_AVATAR_SIZE)
        nWidth = RABBITIM_AVATAR_SIZE;
    if(nHeight > RABBITIM_AVATAR_SIZE)
        nHeight = RABBITIM_AVATAR_SIZE;
    map = pixmap.scaled(nWidth, nHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImageWriter imageWriter(&m_PhotoBuffer, "png");
    m_PhotoBuffer.open(QIODevice::WriteOnly);
    if(!imageWriter.write(map.toImage()))
        LOG_MODEL_ERROR("CDlgRegister", "error:%s", imageWriter.errorString().toStdString().c_str());
    m_PhotoBuffer.close();

    ui->lbPhoto->setPixmap(map);
}

void CDlgRegister::on_pbClear_clicked()
{
    ui->lbPhoto->clear();
    m_PhotoBuffer.setData(NULL, 0);
}
