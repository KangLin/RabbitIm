#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QIcon>
#include "qxmpp/QXmppRosterManager.h"
#include "qxmpp/QXmppMucManager.h"
#include "Widgets/FrmAbout/FrmAbout.h"
#include <QMessageBox>
#include "Widgets/FrmUserList/FrmUserList.h"
#include "Widgets/FrmLogin/FrmLogin.h"
#include "Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Widgets/FrmOptions/FrmOptions.h"
#include "Global.h"
#include "Widgets/FrmSendFile/DlgSendManage.h"
#include "Widgets/FrmUservCard/FrmUservCard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_TrayIcon(QIcon(":/icon/AppIcon"), this),
    m_TrayIconMenu(this),
    m_ActionGroupStatus(this),
    m_ActionGroupTranslator(this),
    ui(new Ui::MainWindow)
{
    CGlobal::Instance()->SetMainWindow(this);

    m_pTranslatorApp = NULL;
    m_pTranslatorQt = NULL;
    m_pTableMain = NULL;
    m_bLogin = false;

    ui->setupUi(this);

    bool check;
    Q_UNUSED(check);
    check = connect(ui->actionAbout_A, SIGNAL(triggered()),
            SLOT(About()));
    Q_ASSERT(check);

    LoadTranslate();
    ReInitMenuOperator();

    //初始化子窗体
    m_pLogin = new CFrmLogin(this);
    this->setCentralWidget(m_pLogin);

    CXmppClient* pClient = CGlobal::Instance()->GetXmppClient();
    if(pClient)
    {
        //初始化qxmpp log
        pClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);

        check = connect(pClient, SIGNAL(disconnected()),
                        SLOT(clientDisconnected()));
        Q_ASSERT(check);

        check = connect(pClient, SIGNAL(error(QXmppClient::Error)),
                        SLOT(clientError(QXmppClient::Error)));
        Q_ASSERT(check);

        /*check = connect(pClient, SIGNAL(iqReceived(QXmppIq)),
                        SLOT(clientIqReceived(QXmppIq)));
        Q_ASSERT(check);//*/

        check = connect(pClient, SIGNAL(stateChanged(QXmppClient::State)),
                        SLOT(stateChanged(QXmppClient::State)));
        Q_ASSERT(check);

        check = connect(&pClient->vCardManager(), SIGNAL(clientVCardReceived()),
                        SLOT(slotClientVCardReceived()));
        Q_ASSERT(check);

        //0712文件发送管理窗口
        //TODO:有内存泄漏
        m_pSendManageDlg = new CDlgSendManage(0);
        //0712处理文件接收信号
        connect(&(pClient->m_TransferManager),SIGNAL(fileReceived(QXmppTransferJob*)),this,SLOT(onReceiveFile(QXmppTransferJob*)));

        CFrmVideo::instance();
    }

    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        check = connect(&m_TrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                        SLOT(slotTrayIconActive(QSystemTrayIcon::ActivationReason)));
        Q_ASSERT(check);

        check = connect(&m_TrayIcon, SIGNAL(messageClicked()),
                        SLOT(slotMessageClicked()));
        Q_ASSERT(check);

        check = connect(&m_TrayIconMenu, SIGNAL(aboutToShow()),
                        SLOT(slotTrayIconMenuUpdate()));
        Q_ASSERT(check);

        check = connect(&m_TrayTimer, SIGNAL(timeout()),
                        SLOT(slotTrayTimeOut()));
        Q_ASSERT(check);

        m_bTrayShow = true;
        m_TrayIcon.setContextMenu(&m_TrayIconMenu);
        m_TrayIcon.setToolTip(tr("RabbitIm"));
        m_TrayIcon.setIcon(QIcon(":/icon/AppIcon"));
        m_TrayIcon.show();
    }
}

MainWindow::~MainWindow()
{
    if(m_pLogin)
        delete m_pLogin;

    //TODO:可能会引起程序core  
    //emit sigRemoveMenu(ui->menuOperator_O);

    if(m_pTableMain)
        delete m_pTableMain;

    delete ui;

    if(m_pTranslatorApp)
        delete m_pTranslatorApp;
    if(m_pTranslatorQt)
        delete m_pTranslatorQt;
}

CRoster* MainWindow::GetRoster(QString szJid)
{
    if(m_pTableMain)
        return m_pTableMain->GetRoster(szJid);
    return NULL;
}

CFrmGroupChat* MainWindow::GetGroupChat(const QString &jid)
{
    if(m_pTableMain)
        return m_pTableMain->GetGroupChat(jid);
    return NULL;
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());

    if(m_pTableMain)
        m_pTableMain->resize(this->geometry().size());
}

void MainWindow::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::showEvent");
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!m_bLogin)
    {
        QApplication::quit();
        return;
    }

    LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent");
    QMessageBox msg(QMessageBox::Question,
                    tr("Close"),
                    tr("Is close the programe?"),
                    QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == msg.exec())
    {
        //退出程序
        e->accept();
        QApplication::closeAllWindows();
    }
    else
        e->ignore(); //忽略退出事件 
}

void MainWindow::changeEvent(QEvent *e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::changeEvent.e->type:%d", e->type());
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void MainWindow::clientConnected()
{
    //关闭登录对话框  
    if(m_pLogin)
    {
        m_pLogin->SaveConf();
        m_pLogin->close();
        delete m_pLogin;
        m_pLogin = NULL;
    }

    if(NULL == m_pTableMain)
        m_pTableMain = new CFrmMain(this);
    if(m_pTableMain)
        this->setCentralWidget(m_pTableMain);
    else
        LOG_MODEL_ERROR("MainWindow", "new CWdgTableMain fail");

    InitLoginedMenu();

    //得到本地用户的详细信息  
    CGlobal::Instance()->GetXmppClient()->vCardManager().requestClientVCard();

    m_bLogin = true;
}

void MainWindow::clientDisconnected()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: DISCONNECTED");
    m_bLogin = false;
}

void MainWindow::clientError(QXmppClient::Error e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: Error:%d", e);
    if(!m_pLogin)
        return;

    if(QXmppClient::SocketError == e)
    {
        m_pLogin->SetPrompt(tr("Network error"));
    }
    else if(QXmppClient::XmppStreamError == e)
    {
        m_pLogin->SetPrompt(tr("User name or password error"));

    }
    else if(QXmppClient::KeepAliveError == e)
        m_pLogin->SetPrompt(tr("Keep Alive error"));
    else
        m_pLogin->SetPrompt(tr("Login success"));

}

void MainWindow::clientIqReceived(const QXmppIq &iq)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: iq Received:%d", iq.error().condition());
}

void MainWindow::slotClientVCardReceived()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotClientVCardReceived");
    CGlobal::Instance()->GetRoster()->SetVCard(CGlobal::Instance()->GetXmppClient()->vCardManager().clientVCard());
    m_TrayIcon.setToolTip(tr("RabbitIm: %1").arg(CGlobal::Instance()->GetShowName()));
}

void MainWindow::stateChanged(QXmppClient::State state)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::stateChanged, state:%d", state);

    //TODO:同一账户在不同地方登录。QXMPP没有提供错误状态  

    /*if(e.xmppStreamError().condition()
            == QXmppStanza::Error::Conflict)
    {
        QMessageBox msg(QMessageBox::QMessageBox::Critical,
                        tr("Error"),
                        tr("The user had logined in other place"),
                        QMessageBox::Ok);

        if(NULL == m_pLogin)
            m_pLogin = new CFrmLogin;

        if(m_pLogin)
        {
            this->setCentralWidget(m_pLogin);
        }
    }*/
}

void MainWindow::sendFile(const QString &jid, const QString &fileName, MainWindow::SendFileType type)
{
    QXmppTransferJob* job = CGlobal::Instance()->GetXmppClient()->m_TransferManager.sendFile(jid,fileName,QString::number(type));
    if(job)
    {
        m_pSendManageDlg->addFileProcess(*job,true);
    }
}

int MainWindow::ReInitMenuOperator()
{
    ui->menuOperator_O->clear();

    //初始化状态菜单  
    ClearMenuStatus();
    InitMenuStatus();

    //初始化翻译菜单  
    ClearMenuTranslate();
    InitMenuTranslate();

    if(m_bLogin)
        InitLoginedMenu();
    else
        InitOperatorMenu();
    return 0;
}

int MainWindow::InitLoginedMenu()
{
    ui->menuOperator_O->clear();
    emit sigMenuRemoveOperator(ui->menuOperator_O);

    ui->menuOperator_O->addMenu(&m_MenuStatus);
    ui->menuOperator_O->addAction(QIcon(":/icon/AppIcon"),
                tr("Edit Locale User Infomation(&E)"),
                this, SLOT(slotEditInformation()));

    //注册菜单  
    emit sigMenuInitOperator(ui->menuOperator_O);

    InitOperatorMenu();
    return 0;
}

int MainWindow::InitOperatorMenu()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitOperatorMenu");
    ui->menuOperator_O->addAction(tr("Change Style Sheet(&S)"), 
                this, SLOT(on_actionChange_Style_Sheet_S_triggered()));
    ui->menuOperator_O->addMenu(&m_MenuTranslate);
    ui->menuOperator_O->addSeparator();
    ui->menuOperator_O->addAction(QIcon(":/icon/Close"), 
                                  tr("Close(&E)"),
                                  this, SLOT(close()));
    return 0;
}

int MainWindow::InitMenuStatus()
{
    m_ActionStatus[QXmppPresence::Online] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Online)),
                             CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Online));
    m_ActionStatus[QXmppPresence::Chat] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Chat)),
                             CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Chat));
    m_ActionStatus[QXmppPresence::Away] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Away)),
                             CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Away));
    m_ActionStatus[QXmppPresence::DND] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::DND)),
                             CGlobal::Instance()->GetRosterStatusText(QXmppPresence::DND));
    m_ActionStatus[QXmppPresence::Invisible] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(QXmppPresence::Invisible)),
                             CGlobal::Instance()->GetRosterStatusText(QXmppPresence::Invisible));

    QMap<QXmppPresence::AvailableStatusType, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
        m_ActionGroupStatus.addAction(it.value());

    bool check = connect(&m_ActionGroupStatus, SIGNAL(triggered(QAction*)),
                         SLOT(slotActionGroupStatusTriggered(QAction*)));
    Q_ASSERT(check);

    QAction* pAct = m_ActionStatus[CGlobal::Instance()->GetStatus()];
    if(pAct)
    {
        pAct->setCheckable(true);
        pAct->setChecked(true);
    }

    m_MenuStatus.setTitle(tr("Status(&S)"));
    m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CGlobal::Instance()->GetStatus())));

    return 0;
}

int MainWindow::ClearMenuStatus()
{
    QMap<QXmppPresence::AvailableStatusType, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
        m_ActionGroupStatus.removeAction(it.value());
    m_ActionGroupStatus.disconnect();
    m_ActionStatus.clear();
    m_MenuStatus.clear();
    return 0;
}

int MainWindow::InitMenuTranslate()
{
    m_MenuTranslate.setTitle(tr("Language(&L)"));
    m_ActionTranslator["Default"] = m_MenuTranslate.addAction(tr("Default"));
    m_ActionTranslator["English"] = m_MenuTranslate.addAction(tr("English"));
    m_ActionTranslator["zh_CN"] = m_MenuTranslate.addAction(tr("Chinese"));

    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
        m_ActionGroupTranslator.addAction(it.value());

    LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitMenuTranslate m_ActionTranslator size:%d", m_ActionTranslator.size());

    bool check = connect(&m_ActionGroupTranslator, SIGNAL(triggered(QAction*)),
                        SLOT(slotActionGroupTranslateTriggered(QAction*)));
    Q_ASSERT(check);

    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", QLocale::system().name()).toString();
    QAction* pAct = m_ActionTranslator[szLocale];
    if(pAct)
    {
        LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitMenuTranslate setchecked locale:%s", szLocale.toStdString().c_str());
        pAct->setCheckable(true);
        pAct->setChecked(true);
        LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitMenuTranslate setchecked end");
    }

    return 0;
}

int MainWindow::ClearMenuTranslate()
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        m_ActionGroupTranslator.removeAction(it.value());
    }
    m_ActionGroupTranslator.disconnect();
    m_ActionTranslator.clear();
    m_MenuTranslate.clear();    

    LOG_MODEL_DEBUG("MainWindow", "MainWindow::ClearMenuTranslate m_ActionTranslator size:%d", m_ActionTranslator.size());
    
    return 0;
}

int MainWindow::LoadTranslate(QString szLocale)
{
    //初始化翻译  
    if(szLocale.isEmpty())
    {
        QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
        szLocale = conf.value("Global/Language", QLocale::system().name()).toString();
    }

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    LOG_MODEL_DEBUG("main", "locale language:%s", szLocale.toStdString().c_str());

    if(m_pTranslatorQt)
    {
        qApp->removeTranslator(m_pTranslatorQt);
        delete m_pTranslatorQt;
    }

    LOG_MODEL_DEBUG("MainWindow", "Translate dir:%s", qPrintable(CGlobal::Instance()->GetDirTranslate()));

    m_pTranslatorQt = new QTranslator(this);
    m_pTranslatorQt->load("qt_" + szLocale, CGlobal::Instance()->GetDirTranslate());
    qApp->installTranslator(m_pTranslatorQt);

    if(m_pTranslatorApp)
    {
        qApp->removeTranslator(m_pTranslatorApp);
        delete m_pTranslatorApp;
    }
    m_pTranslatorApp = new QTranslator(this);
    m_pTranslatorApp->load("app_" + szLocale, CGlobal::Instance()->GetDirTranslate());
    qApp->installTranslator(m_pTranslatorApp);

    ui->retranslateUi(this);
    return 0;
}

void MainWindow::slotActionGroupTranslateTriggered(QAction *pAct)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotActionGroupTranslateTriggered");
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        if(it.value() == pAct)
        {
            QString szLocale = it.key();
            QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
            conf.setValue("Global/Language", szLocale);
            LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotActionGroupTranslateTriggered:%s", it.key().toStdString().c_str());
            LoadTranslate(it.key());
            pAct->setCheckable(true);
            pAct->setChecked(true);
            QMessageBox::information(this, tr("Information"), tr("Change language must reset program."));
            close();
            //ReInitMenuOperator();
            return;
        }
    }
}

void MainWindow::slotTrayIconActive(QSystemTrayIcon::ActivationReason e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::TrayIconActive:%d", e);
    if(QSystemTrayIcon::Trigger == e)
    {
        slotMessageClicked();
        slotTrayTimerStop();
    }
}

void MainWindow::slotMessageClicked()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotMessageClicked");
    if(m_pTableMain)
        m_pTableMain->ShowMessageDialog();
    slotTrayTimerStop();
}

//在通知栏上显示消息  
int MainWindow::ShowTrayIconMessage(const QString &szTitle, const QString &szMessage)
{
    if(CGlobal::Instance()->IsNotifiationBarShowMessage())
    {
        m_TrayIcon.showMessage(szTitle,
                               szMessage, 
                               QSystemTrayIcon::Information,
                               CGlobal::Instance()->GetNotifiationBarShowMessageDelay());
    }
    if(CGlobal::Instance()->IsNotifiationFlashs())
        slotTrayTimerStart();

    if(CGlobal::Instance()->GetMessageSound())
        QSound::play(":/sound/Message");

    return 0;
}

void MainWindow::slotTrayIconMenuUpdate()
{
    m_TrayIconMenu.clear();

    //状态子菜单  
    if(m_bLogin)
    {
        m_TrayIconMenu.addMenu(&m_MenuStatus);

        m_TrayIconMenu.addAction(QIcon(":/icon/AppIcon"),
                    tr("Edit Locale User Infomation(&E)"),
                    this, SLOT(slotEditInformation()));
    }

    m_TrayIconMenu.addAction(ui->actionOptions_O);
    m_TrayIconMenu.addMenu(&m_MenuTranslate);

    QString szTitle;
    if(this->isHidden())
        szTitle = tr("Show Main Windows");
    else
        szTitle = tr("Hide Main Windows");

    m_TrayIconMenu.addAction(szTitle, this, SLOT(on_actionNotifiation_show_main_windows_triggered()));

    m_TrayIconMenu.addSeparator();
    m_TrayIconMenu.addAction(QIcon(":/icon/Close"), tr("Close"), this, SLOT(close()));
}

void MainWindow::slotTrayTimeOut()
{
    if(m_bTrayShow)
        m_TrayIcon.setIcon(QIcon());
    else
        m_TrayIcon.setIcon(QIcon(":/icon/AppIcon"));
    m_bTrayShow = !m_bTrayShow;
}

void MainWindow::slotTrayTimerStart()
{
    m_TrayTimer.start(CGlobal::Instance()->GetNotifiationFlashInterval());
}

void MainWindow::slotTrayTimerStop()
{
    m_TrayIcon.setIcon(QIcon(":/icon/AppIcon"));
    m_TrayTimer.stop();
    m_TrayIcon.show();
}

void MainWindow::on_actionNotifiation_show_main_windows_triggered()
{
    if(isHidden())
        this->show();
    else
        this->hide();
}

void MainWindow::slotActionGroupStatusTriggered(QAction *act)
{
    QMap<QXmppPresence::AvailableStatusType, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        if(it.value() == act)
        {
            QXmppPresence presence;
            QXmppPresence::AvailableStatusType status = it.key();
            CGlobal::Instance()->SetStatus(status);
            CGlobal::Instance()->GetXmppClient()->setClientPresence(presence);
            act->setCheckable(true);
            act->setChecked(true);
            m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(status)));
        }
    }
}

void MainWindow::slotEditInformation()
{
    CFrmUservCard* pvCard = 
            new CFrmUservCard(CGlobal::Instance()->GetRoster(), 
                              true);
    pvCard->show();
    pvCard->activateWindow();
}

void MainWindow::onReceiveFile(QXmppTransferJob *job)
{
    if(job)
    {
        m_pSendManageDlg->addFileProcess(*job,false);
    }
}

void MainWindow::on_actionOptions_O_triggered()
{
    CFrmOptions* pFrm = CFrmOptions::Instance();//是一个单例  
    if(pFrm)
    {
        bool check = connect(pFrm, SIGNAL(sigRefresh()), SIGNAL(sigRefresh()));
        Q_ASSERT(check);

        pFrm->show();
        pFrm->activateWindow();
    }
}

void MainWindow::About()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::About");
    CFrmAbout* pAbout = new CFrmAbout;//CFrmAbout 会在关闭时自动释放内存  
    if(pAbout)
    {
        pAbout->show();
        pAbout->activateWindow();
    }
}

void MainWindow::on_actionChange_Style_Sheet_S_triggered()
{
    //*从资源中加载应用程序样式  
    QString szFile = QFileDialog::getOpenFileName(
                this, tr("Open File"), 
                QString(), "*.qss", 0,
                QFileDialog::ReadOnly | QFileDialog::DontUseNativeDialog);
    if(szFile.isEmpty())
        return;

    QFile file(szFile);//从资源文件中加载  
    if(file.open(QFile::ReadOnly))
    {
        QString stylesheet= file.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
        QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
        conf.setValue("UI/StyleSheet", szFile);
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d", 
                        szFile.toStdString().c_str(), file.error());
    }//*/
}
