#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global.h"
#include "FrmLoginSettings.h"

extern CGlobal g_Global;

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLogin)
{
    LOG_MODEL_DEBUG("Login", "CFrmLogin::CFrmLogin");
    ui->setupUi(this);

    ui->lbePrompt->setText("");

    m_pRegister = new CFrmRegister();

    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    //加载所有用户  
    int userTotal = conf.value("Login/UserTotal", 0).toInt();
    for(int i = 0; i < userTotal; i++)
    {
        ui->cmbUser->addItem(conf.value(QString("Login/UserName") + QString::number(i +1) ).toString());        
    }

    //最后一次登录用户  
    int nIndex = conf.value("Login/LastUserNameIndex").toInt();
    ui->cmbUser->setCurrentIndex(nIndex);

    ui->lnPassword->setText(conf.value("Login/Password" + QString::number(nIndex + 1), "").toString());
    if(ui->lnPassword->text() != "" || !ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(true);
    else
        ui->chkSave->setChecked(false);

    ui->chkLogin->setChecked(conf.value("Login/AutoLogin", false).toBool());

    InitStateButton();

    if(ui->chkLogin->checkState() == Qt::Checked)
    {
        m_tmAutoLogin.start(1000);
        bool check = connect(&m_tmAutoLogin, SIGNAL(timeout()), SLOT(on_pbOk_clicked()));
        Q_ASSERT(check);
    }
}

CFrmLogin::~CFrmLogin()
{
    delete ui;

    if(m_pRegister)
        delete m_pRegister;

    LOG_MODEL_DEBUG("Login", "CFrmLogin::~CFrmLogin");
}

void CFrmLogin::on_pbOk_clicked()
{
    if(m_tmAutoLogin.isActive())
        m_tmAutoLogin.stop();

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
    config.setHost(g_Global.GetXmppServerHost());
    config.setPort(g_Global.GetXmppServerPort());
    config.setDomain(g_Global.GetXmppServer());
    config.setUser(ui->cmbUser->currentText());
    config.setPassword(ui->lnPassword->text());
    g_Global.SetJid(config.jid());

    LOG_MODEL_DEBUG("Login", "Local jid:%s;config.jidBare():%s",
           qPrintable(g_Global.GetBareJid()),
           qPrintable(config.jidBare()));
    QXmppPresence presence;
    presence.setAvailableStatusType(m_Status);
    ((MainWindow*)(this->parent()))->m_pClient->connectToServer(config, presence);
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

void CFrmLogin::on_pbSet_clicked()
{
    CFrmLoginSettings* pSet = new CFrmLoginSettings();//窗口关闭时，会自动释放内存  
    if(pSet)
    {
        pSet->SetLogin(this);
        pSet->show();
        pSet->activateWindow();
    }
    else
        LOG_MODEL_ERROR("Login", "new CFrmLoginSettings fail");
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
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
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
        if(ui->chkLogin->isChecked() || ui->chkSave->isChecked())
        {
            conf.setValue("Login/Password" + QString::number(total +1), EncryptPassword(ui->lnPassword->text()));
        }
        else
            conf.setValue("Login/Password" + QString::number(total +1), "");
    }
    return 0;
}

QString CFrmLogin::EncryptPassword(QString szPassword)
{
    return szPassword;
}

QString CFrmLogin::DecryptPassword(QString szPassword)
{
    return szPassword;
}

void CFrmLogin::on_chkLogin_stateChanged(int state)
{
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    if(Qt::Unchecked == state)
    {
        conf.setValue("Login/AutoLogin", false);
    }
    if(Qt::Checked == state)
            conf.setValue("Login/AutoLogin", true);
}

int CFrmLogin::InitStateButton()
{
    QAction* pAction = m_StateMenu.addAction(tr("OnLine"));
    bool check = connect(pAction, SIGNAL(triggered()), SLOT(slotOnLineTriggered()));
    Q_ASSERT(check);

    pAction = m_StateMenu.addAction(tr("Chat"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotChatTriggered()));
    Q_ASSERT(check);

    pAction = m_StateMenu.addAction(tr("Temporarily away"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotTemporarilyawayTriggered()));
    Q_ASSERT(check);

    pAction = m_StateMenu.addAction(tr("Do not disturb"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotDoNotDisturbTriggered()));
    Q_ASSERT(check);

    pAction = m_StateMenu.addAction(tr("Invisible"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotInvisibleTriggered()));
    Q_ASSERT(check);

    ui->pbState->setMenu(&m_StateMenu);

    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    m_Status = (QXmppPresence::AvailableStatusType)conf.value("Login/LoginState", QXmppPresence::Online).toInt();
    ui->pbState->setText(g_Global.GetStatusText(m_Status));
    return 0;
}

void CFrmLogin::slotChatTriggered()
{
    m_Status = QXmppPresence::Chat;
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", m_Status);
    ui->pbState->setText(tr("Chat"));
}

void CFrmLogin::slotOnLineTriggered()
{
    m_Status = QXmppPresence::Online;
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", m_Status);
    ui->pbState->setText(tr("OnLine"));
}

void CFrmLogin::slotDoNotDisturbTriggered()
{
    m_Status = QXmppPresence::DND;
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", m_Status);
    ui->pbState->setText(tr("Do not disturb"));
}

void CFrmLogin::slotInvisibleTriggered()
{
    m_Status = QXmppPresence::Invisible;
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", m_Status);
    ui->pbState->setText(tr("Invisible"));
}

void CFrmLogin::slotTemporarilyawayTriggered()
{
    m_Status = QXmppPresence::Away;
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", m_Status);
    ui->pbState->setText(tr("Temporarily away"));
}

void CFrmLogin::on_cmbUser_currentIndexChanged(int index)
{
    QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
    ui->lnPassword->setText(conf.value("Login/Password" + QString::number(index + 1), "").toString());
    if(ui->lnPassword->text() == "" || ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(false);
    else
        ui->chkSave->setChecked(true);
}
