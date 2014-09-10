#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include "../../Global/Global.h"
#include "FrmLoginSettings.h"

extern CGlobal g_Global;

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    m_StateMenu(this),
    m_ActionGroupStatus(this),
    ui(new Ui::CFrmLogin)
{
    LOG_MODEL_DEBUG("Login", "CFrmLogin::CFrmLogin");
    ui->setupUi(this);

    ui->lbePrompt->setText("");

    m_pRegister = new CFrmRegister();

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

    ui->lnPassword->setText(conf.value("Login/Password" + QString::number(nIndex + 1), "").toString());
    if(ui->lnPassword->text() != "" || !ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(true);
    else
        ui->chkSave->setChecked(false);

    ui->chkLogin->setChecked(CGlobal::Instance()->GetAutoLogin());

    ReinitStateButton();

    if(ui->chkLogin->checkState() == Qt::Checked)
    {
        m_tmAutoLogin.start(1000 * CGlobal::Instance()->GetAutoLoginDelayTime());
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

    bool check = connect(CGlobal::Instance()->GetXmppClient(),
                         SIGNAL(connected()),
                         (MainWindow*)(this->parent()),
                         SLOT(slotClientConnected()));
    Q_ASSERT(check);

    ui->lbePrompt->setText(tr("Being Login..."));

    QXmppConfiguration config;
    //TODO:设置为非sasl验证  
    config.setUseSASLAuthentication(false);
    //config.setUseNonSASLAuthentication(false);
    config.setHost(CGlobal::Instance()->GetXmppServer());
    config.setPort(CGlobal::Instance()->GetXmppServerPort());
    config.setDomain(CGlobal::Instance()->GetXmppDomain());
    config.setUser(ui->cmbUser->currentText());
    config.setPassword(ui->lnPassword->text());

    QXmppPresence presence;
    presence.setAvailableStatusType(m_Status);
    CGlobal::Instance()->GetXmppClient()->connectToServer(config, presence);
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
        if(ui->chkLogin->isChecked() || ui->chkSave->isChecked())
        {
            conf.setValue("Login/Password" + QString::number(total +1), EncryptPassword(ui->lnPassword->text()));
        }
        else
            conf.setValue("Login/Password" + QString::number(total +1), "");
    }
    return 0;
}

//TODO:
QString CFrmLogin::EncryptPassword(QString szPassword)
{
    return szPassword;
}
//TODO:
QString CFrmLogin::DecryptPassword(QString szPassword)
{
    return szPassword;
}

void CFrmLogin::on_chkLogin_stateChanged(int state)
{
    Q_UNUSED(state);
    CGlobal::Instance()->SetAutoLogin(ui->chkLogin->isChecked());
}

int CFrmLogin::ReinitStateButton()
{
    QMap<QXmppPresence::AvailableStatusType, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        m_ActionGroupStatus.removeAction(it.value());
    }    
    m_ActionStatus.clear();
    m_StateMenu.clear();

    m_ActionStatus[QXmppPresence::Online] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Online)),
                                  CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Online));

    m_ActionStatus[QXmppPresence::Chat] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Chat)),
                                  CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Chat));

    m_ActionStatus[QXmppPresence::Away] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Away)),
                                  CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Away));

    m_ActionStatus[QXmppPresence::DND] = 
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::DND)),
                                  CGlobal::Instance()->GetRosterStatusText(QXmppPresence::DND));

    m_ActionStatus[QXmppPresence::Invisible] =
            m_StateMenu.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Invisible)),
                                    CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Invisible));

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
    QMap<QXmppPresence::AvailableStatusType, QAction*>::iterator it;
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
    ui->lnPassword->setText(conf.value("Login/Password" + QString::number(index + 1), "").toString());
    if(ui->lnPassword->text() == "" || ui->lnPassword->text().isEmpty())
        ui->chkSave->setChecked(false);
    else
        ui->chkSave->setChecked(true);
}
