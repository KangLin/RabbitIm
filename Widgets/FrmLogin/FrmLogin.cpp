#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global/Global.h"
#include "DlgLoginSettings.h"
#include <QSettings>
#include "MainWindow.h"
#include "Global/Encrypt.h"
#include <string>

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    m_StateMenu(this),
    m_ActionGroupStatus(this),
    ui(new Ui::CFrmLogin)
{
    LOG_MODEL_DEBUG("Login", "CFrmLogin::CFrmLogin");
    ui->setupUi(this);

    ui->lbePrompt->setText("");

    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    //加载所有用户  
    int userTotal = conf.value("Login/UserTotal", 0).toInt();
    for(int i = 0; i < userTotal; i++)
    {
        ui->cmbUser->addItem(conf.value(QString("Login/UserName") + QString::number(i +1) ).toString());
    }

    //最后一次登录用户  
    int nIndex = conf.value("Login/LastUserNameIndex").toInt();
    ui->cmbUser->setCurrentIndex(nIndex);

    ui->lnPassword->setText(DecryptPassword(conf.value("Login/Password" + QString::number(nIndex + 1), "").toString()));
    if(ui->lnPassword->text() != "" || !ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(true);
    else
        ui->chkSave->setChecked(false);

    ui->chkLogin->setChecked(CGlobal::Instance()->GetAutoLogin());

    ReinitStateButton();

    if(ui->chkLogin->checkState() == Qt::Checked)
    {
        bool check = connect(&m_tmAutoLogin, SIGNAL(timeout()), SLOT(on_pbOk_clicked()));
        Q_ASSERT(check);
        m_tmAutoLogin.start(1000 * CGlobal::Instance()->GetAutoLoginDelayTime());
    }
}

CFrmLogin::~CFrmLogin()
{
    delete ui;

    LOG_MODEL_DEBUG("Login", "CFrmLogin::~CFrmLogin");
}

void CFrmLogin::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->pbState->setText(CGlobal::Instance()->GetRosterStatusText(m_Status));
        this->ReinitStateButton();
        break;
    }
}

void CFrmLogin::on_pbOk_clicked()
{
    if(m_tmAutoLogin.isActive())
        m_tmAutoLogin.stop();

    disconnect(GET_CLIENT.data(), SIGNAL(sigClientConnected()),
               GET_MAINWINDOW,
               SLOT(slotClientConnected()));
    bool check = connect(GET_CLIENT.data(),
                         SIGNAL(sigClientConnected()),
                        GET_MAINWINDOW,
                         SLOT(slotClientConnected()));
    Q_ASSERT(check);
    disconnect(GET_CLIENT.data(), SIGNAL(sigClientError(CClient::ERROR_TYPE)),
               this, SLOT(slotClientError(CClient::ERROR_TYPE)));
    check = connect(GET_CLIENT.data(), SIGNAL(sigClientError(CClient::ERROR_TYPE)),
                    SLOT(slotClientError(CClient::ERROR_TYPE)));
    Q_ASSERT(check);
    ui->lbePrompt->setText(tr("Being Login..."));

    GET_CLIENT->Login(ui->cmbUser->currentText(), ui->lnPassword->text(), m_Status);
    return;
}

void CFrmLogin::on_pbClose_clicked()
{
    //退出程序  
    ((QWidget*)this->parent())->close();
}

void CFrmLogin::on_pbRegitster_clicked()
{
    disconnect(GET_CLIENT.data(), SIGNAL(sigClientConnected()),
               CGlobal::Instance()->GetMainWindow(),
               SLOT(slotClientConnected()));
    CDlgRegister r(this);
    r.exec();
}

void CFrmLogin::on_pbSet_clicked()
{
    CDlgLoginSettings set(this);//窗口关闭时，会自动释放内存  
    set.exec();
}

int CFrmLogin::SetPrompt(QString szPrompt)
{
    int nRet = 0;
    ui->lbePrompt->setText(szPrompt);
    return nRet;
}

int CFrmLogin::SetLoginInformation(QString szName, QString szPassword)
{
    ui->cmbUser->setCurrentText(szName);
    ui->lnPassword->setText(szPassword);
    return 0;
}

int CFrmLogin::SaveConf()
{
    LOG_MODEL_DEBUG("Login", "CFrmLogin::SaveConf");
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    int total = conf.value("Login/UserTotal", 0).toInt();
    int i = 0;
    for(i = 0; i < total; i++)
    {
        if(conf.value("Login/UserName" + QString::number(i + 1)) == ui->cmbUser->currentText())
        {
            conf.setValue("Login/LastUserNameIndex",  i);//设置最后一次登录用户的索引  
            if(ui->chkLogin->isChecked() || ui->chkSave->isChecked())
            {
                conf.setValue("Login/Password" + QString::number(i +1), EncryptPassword(ui->lnPassword->text()));
            }
            else
                conf.setValue("Login/Password" + QString::number(i +1), "");
            return 0;
        }
    }

    if(i >= total)
    {
        conf.setValue("Login/UserTotal", total + 1);
        conf.setValue("Login/UserName" + QString::number(total +1), ui->cmbUser->currentText());
        conf.setValue("Login/LastUserNameIndex",  i);//设置最后一次登录用户的索引  
        if(ui->chkLogin->isChecked() || ui->chkSave->isChecked())
        {
            conf.setValue("Login/Password" + QString::number(total +1), EncryptPassword(ui->lnPassword->text()));
        }
        else
            conf.setValue("Login/Password" + QString::number(total +1), "");
    }
    return 0;
}

QString gPassword("RabbitIm.KangLin");
//加密密码  
QString CFrmLogin::EncryptPassword(QString szPassword)
{
#ifdef RABBITIM_USER_OPENSSL
    CEncrypt  e;
    char* pOut = NULL;
    int nLen = 0;
    e.SetPassword(gPassword.toStdString().c_str());
    e.Encode(szPassword.toStdString().c_str(), szPassword.toStdString().size(), &pOut, nLen);
    QByteArray ba(pOut, nLen);
    return ba.toHex();
#else
    return szPassword;
#endif
}

//解密密码  
QString CFrmLogin::DecryptPassword(QString szPassword)
{
    if(szPassword.isEmpty())
        return szPassword;
#ifdef RABBITIM_USER_OPENSSL
    CEncrypt  e;
    QByteArray ba;
    ba = QByteArray::fromHex(QByteArray(szPassword.toStdString().c_str(), szPassword.toStdString().size()));
    std::string szOut;
    e.SetPassword(gPassword.toStdString().c_str());
    e.Dencode(ba.data(), ba.length(), szOut);
    return szOut.c_str();
#else
    return szPassword;
#endif
}

void CFrmLogin::on_chkLogin_stateChanged(int state)
{
    Q_UNUSED(state);
    CGlobal::Instance()->SetAutoLogin(ui->chkLogin->isChecked());
}

int CFrmLogin::ReinitStateButton()
{
    QMap<CUserInfo::USER_INFO_STATUS, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        m_ActionGroupStatus.removeAction(it.value());
    }    
    m_ActionStatus.clear();
    m_StateMenu.clear();

    m_ActionStatus[CUserInfo::Online] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Online)),
                                  CGlobal::Instance()->GetRosterStatusText(CUserInfo::Online));

    m_ActionStatus[CUserInfo::Chat] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Chat)),
                                  CGlobal::Instance()->GetRosterStatusText(CUserInfo::Chat));

    m_ActionStatus[CUserInfo::Away] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Away)),
                                  CGlobal::Instance()->GetRosterStatusText(CUserInfo::Away));

    m_ActionStatus[CUserInfo::DO_NOT_DISTURB] = 
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::DO_NOT_DISTURB)),
                                  CGlobal::Instance()->GetRosterStatusText(CUserInfo::DO_NOT_DISTURB));

    m_ActionStatus[CUserInfo::Invisible] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Invisible)),
                                    CGlobal::Instance()->GetRosterStatusText(CUserInfo::Invisible));

    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
        m_ActionGroupStatus.addAction(it.value());

    bool check = connect(&m_ActionGroupStatus, SIGNAL(triggered(QAction*)),
                        SLOT(slotActionGroupStatusTriggered(QAction*)));
    Q_ASSERT(check);

    ui->pbState->setMenu(&m_StateMenu);

    m_Status = CGlobal::Instance()->GetStatus();
    ui->pbState->setText(CGlobal::Instance()->GetRosterStatusText(m_Status));
    ui->pbState->setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(m_Status)));
    return 0;
}

void CFrmLogin::slotActionGroupStatusTriggered(QAction *pAct)
{
    QMap<CUserInfo::USER_INFO_STATUS, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        if(it.value() == pAct)
        {
            m_Status = it.key();
            CGlobal::Instance()->SetStatus(m_Status);
            ui->pbState->setText(CGlobal::Instance()->GetRosterStatusText(m_Status));
            ui->pbState->setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(m_Status)));
        }
    }
}

void CFrmLogin::on_cmbUser_currentIndexChanged(int index)
{
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    ui->lnPassword->setText(this->DecryptPassword(conf.value("Login/Password" + QString::number(index + 1), "").toString()));
    if(ui->lnPassword->text() == "" || ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(false);
    else
        ui->chkSave->setChecked(true);
}

void CFrmLogin::slotRefresh()
{
    ui->chkLogin->setChecked(CGlobal::Instance()->GetAutoLogin());
}

void CFrmLogin::slotClientError(CClient::ERROR_TYPE e)
{
    QString szMsg;
    switch (e)
    {
    case CClient::NetworkError:
        szMsg = tr("Network error");
        break;
    case CClient::KeepAliveError:
        szMsg = tr("Keep active error");
        break;
    case CClient::LoginFail:
        szMsg = tr("Login fail");
    default:
        break;
    }
    SetPrompt(szMsg);
}
