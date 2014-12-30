#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QIcon>
#include "Widgets/DlgAbout/DlgAbout.h"
#include <QMessageBox>
#include "Widgets/FrmUserList/FrmUserList.h"
#include "Widgets/FrmLogin/FrmLogin.h"
#include "Global/Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Widgets/DlgOptions/DlgOptions.h"
#include "Widgets/FrmSendFile/DlgSendManage.h"
#include "Widgets/DlgUservCard/DlgUservCard.h"
#ifdef RABBITIM_USER_LIBCURL
#include "Update/DlgUpdate.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_TrayIcon( this),
    m_TrayIconMenu(this),
    m_ActionGroupStatus(this),
    m_ActionGroupTranslator(this),
    m_ActionGroupStyle(this),
    m_Login(new CFrmLogin(this)),
    ui(new Ui::MainWindow)
{
    CGlobal::Instance()->SetMainWindow(this);
    CTool::SetWindowsGeometry(this);
    ui->setupUi(this);

    m_bLogin = false;

    bool check;
    check = connect(ui->actionAbout_A, SIGNAL(triggered()),
            SLOT(About()));
    Q_ASSERT(check);

    LoadStyle();
    LoadTranslate();
    ReInitMenuOperator();

#ifdef RABBITIM_USER_LIBCURL
    check = connect(this, SIGNAL(sigUpdateExec(int,QString)),
                    SLOT(slotUpdateExec(int,QString)));
    Q_ASSERT(check);
#endif

    //初始化子窗体
    if(!m_Login.isNull())
    {
        check = connect(this, SIGNAL(sigRefresh()),
                        m_Login.data(), SLOT(slotRefresh()));
        Q_ASSERT(check);
        this->setCentralWidget(m_Login.data());
    }
    QSharedPointer<CClient> client = GET_CLIENT;
    if(!client.isNull())
    {
        check = connect(client.data(), SIGNAL(sigClientDisconnected()),
                        SLOT(slotClientDisconnected()));
        Q_ASSERT(check);

        check = connect(client.data(), SIGNAL(sigUpdateLocaleUserInfo()),
                        SLOT(slotUpdateLocaleUserInfo()));
        Q_ASSERT(check);

        check = connect(client.data(), SIGNAL(sigMessageClean(const QString&)),
                        SLOT(slotMessageClean(const QString&)));
        Q_ASSERT(check);

        check = connect(GETMANAGER->GetManageGroupChat().data(),
                        SIGNAL(sigMessageClean(QString)),
                        SLOT(slotMessageClean(const QString&)));
        Q_ASSERT(check);
    }

    //设置应用程序图标  
    this->setWindowIcon(QIcon(":/icon/AppIcon"));
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
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::~MainWindow()");
    //TODO:可能会引起程序core  
    //emit sigRemoveMenu(ui->menuOperator_O);

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::resizeEvent:e.size:%d;genmetry.size:%d,frame.size:%d",
                    e->size().width(),
                    geometry().size().width(),
                    this->frameGeometry().width());

    if(!m_TableMain.isNull())
        m_TableMain->resize(this->geometry().size());
}

void MainWindow::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::showEvent");
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent");
    if(!m_bLogin)
    {
        LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent QApplication::quit()");
        QApplication::quit();
        return;
    }

    LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent start");
    int type = CGlobal::Instance()->GetCloseType();
    switch (type) {
    case CGlobal::E_CLOSE_TYPE_NO:
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Close"),
                        tr("Is close the programe or logout?"),
                        QMessageBox::Ok | QMessageBox::Yes | QMessageBox::Cancel);
        msg.setButtonText(QMessageBox::Ok , tr("close"));
        msg.setButtonText(QMessageBox::Yes, tr("logout"));
        msg.setDefaultButton(QMessageBox::Yes);
        /*QMessageBox msg;
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle(tr("Close"));
        msg.setText(tr("Is close the programe or logout?"));
        QPushButton* pLogout = msg.addButton(tr("Logout"), QMessageBox::YesRole);
        QPushButton * pClose = msg.addButton(tr("Close"), QMessageBox::AcceptRole);
        msg.addButton(tr("Cancel"), QMessageBox::NoRole);
        msg.setDefaultButton(pClose);*/
        int nRet = msg.exec();
        if(QMessageBox::Ok == nRet)
        //if(msg.clickedButton() == pClose)
        {
            CGlobal::Instance()->SetCloseType(CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME);
            //退出程序  
            GET_CLIENT->Logout();
            e->accept();
            QApplication::closeAllWindows();
        }
        //else if(msg.clickedButton() == pLogout)
        else if(QMessageBox::Yes == nRet)
        {
            CGlobal::Instance()->SetCloseType(CGlobal::E_CLOSE_TYPE_LOGOUT);
            e->ignore();
            slotLogout();
        }
        else
            e->ignore(); //忽略退出事件 
    }
        break;
    case CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME:
        //退出程序  
        GET_CLIENT->Logout();
        e->accept();
        QApplication::closeAllWindows();
        break;
    case CGlobal::E_CLOSE_TYPE_LOGOUT:
        slotLogout();
        e->ignore();
        break;
    default:
        e->ignore(); //忽略退出事件 
        break;
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    //LOG_MODEL_DEBUG("MainWindow", "MainWindow::changeEvent.e->type:%d", e->type());
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void MainWindow::slotClientConnected()
{
    //关闭登录对话框  
    if(!m_Login.isNull())
    {
        m_Login->SaveConf();
        m_Login->close();
        m_Login.clear();
    }

    if(m_TableMain.isNull())
    {
        QSharedPointer<CFrmMain> main(new CFrmMain);
        m_TableMain = main;
    }
    if(!m_TableMain.isNull())
     {
        this->setCentralWidget(m_TableMain.data());
    }
    else
        LOG_MODEL_ERROR("MainWindow", "new CWdgTableMain fail");

    m_bLogin = true;
    ReInitMenuOperator();
}

void MainWindow::slotClientDisconnected()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: DISCONNECTED");
    m_bLogin = false;
    if(m_Login.isNull())
    {
        QSharedPointer<CFrmLogin> f(new CFrmLogin(this));
        m_Login = f;
    }
    if(!m_Login.isNull())
    {
        this->setCentralWidget(m_Login.data());
        bool check = connect(this, SIGNAL(sigRefresh()),
                        m_Login.data(), SLOT(slotRefresh()));
        Q_ASSERT(check);
    }
    m_TableMain.clear();

    setWindowTitle("RabbitIm");
    ReInitMenuOperator();
}

void MainWindow::slotUpdateLocaleUserInfo()
{
    this->m_TrayIcon.setToolTip(tr("RabbitIm:%1").arg(GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName()));
    this->setWindowTitle(tr("RabbitIm:%1").arg(GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName()));
    QPixmap pixmap;
    pixmap.convertFromImage(GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetPhoto());
    setWindowIcon(QIcon(pixmap));
}

void MainWindow::sendFile(const QString &jid, const QString &fileName, MainWindow::SendFileType type)
{
    /*QXmppTransferJob* job = XMPP_CLIENT->m_TransferManager.sendFile(jid,fileName,QString::number(type));
    if(job)
    {
        m_pSendManageDlg->addFileProcess(*job,true);
    }*/
}

int MainWindow::ReInitMenuOperator()
{
    ui->menuOperator_O->clear();

    //初始化翻译菜单  
    ClearMenuTranslate();
    InitMenuTranslate();
    //初始化样式菜单  
    ClearMenuStyles();
    InitMenuStyles();

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

    //初始化状态菜单  
    ClearMenuStatus();
    InitMenuStatus();

    ui->menuOperator_O->addMenu(&m_MenuStatus);
    ui->menuOperator_O->addAction(QIcon(":/icon/Information"),
                tr("Edit Locale User Infomation(&E)"),
                this, SLOT(slotEditInformation()));
    //注册菜单  
    emit sigMenuInitOperator(ui->menuOperator_O);
 
    ui->menuOperator_O->addSeparator();

    InitOperatorMenu();
    return 0;
}

int MainWindow::InitOperatorMenu()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitOperatorMenu");
    ui->menuOperator_O->addMenu(&m_MenuStyle);
    ui->menuOperator_O->addMenu(&m_MenuTranslate);
    ui->menuOperator_O->addSeparator();
    if(m_bLogin)
        ui->menuOperator_O->addAction(QIcon(":/icon/Logout"), 
                                      tr("Logout"),
                                      this, SLOT(slotLogout()));
    ui->menuOperator_O->addAction(QIcon(":/icon/Close"), 
                                  tr("Close(&E)"),
                                  this, SLOT(close()));
    return 0;
}

int MainWindow::InitMenuStyles()
{
    m_ActionStyles["Custom"] = m_MenuStyle.addAction(tr("Custom"));
    m_ActionStyles["System"] = m_MenuStyle.addAction(tr("System"));
    m_ActionStyles["Blue"] = m_MenuStyle.addAction(tr("Blue"));
    m_ActionStyles["Dark"] = m_MenuStyle.addAction(tr("Dark"));
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupStyle.addAction(it.value());
    }
    bool check = connect(&m_ActionGroupStyle, SIGNAL(triggered(QAction*)),
                         SLOT(slotActionGroupStyleTriggered(QAction*)));
    Q_ASSERT(check);
    QAction* pAct = m_ActionStyles[CGlobal::Instance()->GetMenuStyle()];
    if(pAct)
    {
        pAct->setChecked(true);
    }
    m_MenuStyle.setIcon(QIcon(":/icon/Stype"));
    m_MenuStyle.setTitle(tr("Change Style Sheet(&S)"));
    return 0;
}

int MainWindow::ClearMenuStyles()
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        m_ActionGroupStyle.removeAction(it.value());
    }
    m_ActionGroupStyle.disconnect();
    m_ActionStyles.clear();
    m_MenuStyle.clear();
    return 0;
}

int MainWindow::InitMenuStatus()
{
    m_ActionStatus[CUserInfo::Online] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Online)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::Online));
    m_ActionStatus[CUserInfo::Chat] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Chat)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::Chat));
    m_ActionStatus[CUserInfo::Away] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Away)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::Away));
    m_ActionStatus[CUserInfo::DO_NOT_DISTURB] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::DO_NOT_DISTURB)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::DO_NOT_DISTURB));
    m_ActionStatus[CUserInfo::Invisible] = 
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::Invisible)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::Invisible));
    m_ActionStatus[CUserInfo::OffLine] =
            m_MenuStatus.addAction(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CUserInfo::OffLine)),
                             CGlobal::Instance()->GetRosterStatusText(CUserInfo::OffLine));

    QMap<CUserInfo::USER_INFO_STATUS, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupStatus.addAction(it.value());
    }
    
    bool check = connect(&m_ActionGroupStatus, SIGNAL(triggered(QAction*)),
                         SLOT(slotActionGroupStatusTriggered(QAction*)));
    Q_ASSERT(check);

    QAction* pAct = m_ActionStatus[CGlobal::Instance()->GetStatus()];
    if(pAct)
    {
        pAct->setChecked(true);
    }

    m_MenuStatus.setTitle(tr("Status(&S)"));
    m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(CGlobal::Instance()->GetStatus())));

    return 0;
}

int MainWindow::ClearMenuStatus()
{
    QMap<CUserInfo::USER_INFO_STATUS, QAction*>::iterator it;
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
    m_MenuTranslate.setIcon(QIcon(":/icon/Language"));
    m_ActionTranslator["Default"] = m_MenuTranslate.addAction(QIcon(":/icon/Language"), tr("Default"));
    m_ActionTranslator["English"] = m_MenuTranslate.addAction(QIcon(":/icon/English"), tr("English"));
    m_ActionTranslator["zh_CN"] = m_MenuTranslate.addAction(QIcon(":/icon/China"), tr("Chinese"));

    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupTranslator.addAction(it.value());
    }

    LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitMenuTranslate m_ActionTranslator size:%d", m_ActionTranslator.size());

    bool check = connect(&m_ActionGroupTranslator, SIGNAL(triggered(QAction*)),
                        SLOT(slotActionGroupTranslateTriggered(QAction*)));
    Q_ASSERT(check);

    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", "Default").toString();
    QAction* pAct = m_ActionTranslator[szLocale];
    if(pAct)
    {
        LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitMenuTranslate setchecked locale:%s", szLocale.toStdString().c_str());
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

    if(!m_TranslatorQt.isNull())
    {
        qApp->removeTranslator(m_TranslatorQt.data());
        m_TranslatorQt.clear();
    }

    if(m_TranslatorApp.isNull())
    {
        qApp->removeTranslator(m_TranslatorApp.data());
        m_TranslatorApp.clear();
    }
    LOG_MODEL_DEBUG("MainWindow", "Translate dir:%s", qPrintable(CGlobal::Instance()->GetDirTranslate()));

    m_TranslatorQt = QSharedPointer<QTranslator>(new QTranslator(this));
    m_TranslatorQt->load("qt_" + szLocale + ".qm", CGlobal::Instance()->GetDirTranslate());
    qApp->installTranslator(m_TranslatorQt.data());

    m_TranslatorApp = QSharedPointer<QTranslator>(new QTranslator(this));
#ifdef ANDROID
    m_TranslatorApp->load(":/translations/app_" + szLocale + ".qm");
#else
    m_TranslatorApp->load("app_" + szLocale + ".qm", CGlobal::Instance()->GetDirTranslate());
#endif
    qApp->installTranslator(m_TranslatorApp.data());

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
        this->show();
        this->activateWindow();
    }
}

void MainWindow::slotMessageClicked()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotMessageClicked");
    CGlobal::Instance()->GetManager()->GetRecentMessage()->ShowLastMessageDialog();
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

        m_TrayIconMenu.addAction(QIcon(":/icon/Information"),
                    tr("Edit Locale User Infomation(&E)"),
                    this, SLOT(slotEditInformation()));
    }

    m_TrayIconMenu.addAction(ui->actionOptions_O);
    m_TrayIconMenu.addMenu(&m_MenuTranslate);

    QString szTitle;
    if(this->isHidden() || this->isMinimized())
        szTitle = tr("Show Main Windows");
    else
        szTitle = tr("Hide Main Windows");

    m_TrayIconMenu.addAction(QIcon(":/icon/AppIcon"), szTitle, this, SLOT(on_actionNotifiation_show_main_windows_triggered()));

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
    if(!m_TrayTimer.isActive())
        m_TrayTimer.start(CGlobal::Instance()->GetNotifiationFlashInterval());
}

void MainWindow::slotTrayTimerStop()
{
    m_TrayIcon.setIcon(QIcon(":/icon/AppIcon"));
    m_TrayTimer.stop();
    m_TrayIcon.show();
}

void MainWindow::slotMessageClean(const QString&)
{
    if(!CGlobal::Instance()->GetManager()->GetRecentMessage()->GetUnreadCount())
        slotTrayTimerStop();
}

void MainWindow::on_actionNotifiation_show_main_windows_triggered()
{
    if(isHidden() || isMinimized())
     {
        this->show();
        this->activateWindow();
    }
    else
        this->hide();
}

void MainWindow::slotActionGroupStatusTriggered(QAction *act)
{
    QMap<CUserInfo::USER_INFO_STATUS, QAction*>::iterator it;
    for(it = m_ActionStatus.begin(); it != m_ActionStatus.end(); it++)
    {
        if(it.value() == act)
        {
            CUserInfo::USER_INFO_STATUS status = it.key();
            USER_INFO_LOCALE->GetInfo()->SetStatus(status);
            GET_CLIENT->setClientStatus(status);
            act->setChecked(true);
            m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(status)));
        }
    }
}

void MainWindow::slotEditInformation()
{
    CDlgUservCard pvCard(USER_INFO_LOCALE->GetInfo(), true);
    pvCard.exec();
}

void MainWindow::slotLogout()
{
    GET_CLIENT->Logout();
}

void MainWindow::on_actionOptions_O_triggered()
{
    CDlgOptions dlg(this);
    dlg.exec();
}

void MainWindow::About()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::About");
    CDlgAbout about(this);
    about.exec();
}

void MainWindow::slotActionGroupStyleTriggered(QAction* act)
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        if(it.value() == act)
        {
            act->setChecked(true);
            if(it.key() == "Blue")
                CGlobal::Instance()->SetMenuStyle("Blue", ":/sink/Blue");
            else if(it.key() == "Dark")
                CGlobal::Instance()->SetMenuStyle("Dark", ":/qdarkstyle/style.qss");
            else if(it.key() == "Custom")
                OpenCustomStyleMenu();
            else
                CGlobal::Instance()->SetMenuStyle("System", "");
        }
    }

    LoadStyle();
}

int MainWindow::LoadStyle()
{
    //*从配置文件中加载应用程序样式  
    QString szFile = CGlobal::Instance()->GetStyle();
    if(szFile.isEmpty())
        qApp->setStyleSheet("");
    else
    {
        QFile file(szFile);//从资源文件中加载  
        if(file.open(QFile::ReadOnly))
        {
            QString stylesheet= file.readAll();
            qApp->setStyleSheet(stylesheet);
            file.close();
        }
        else
        {
            LOG_MODEL_ERROR("app", "file open file [%s] fail:%d",
                            CGlobal::Instance()->GetStyle().toStdString().c_str(), file.error());
        }
    }
    return 0;
}

int MainWindow::OpenCustomStyleMenu()
{
    QString szFile;
    QString szFilter("*.qss *.*");
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(szFile.isEmpty())
        return -1;

    QFile file(szFile);//从资源文件中加载  
    if(file.open(QFile::ReadOnly))
    {
        QString stylesheet= file.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
        QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
        conf.setValue("UI/StyleSheet", szFile);
        
        CGlobal::Instance()->SetMenuStyle("Custom", szFile);
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d", 
                        szFile.toStdString().c_str(), file.error());
    }
    return 0;
}

#ifdef RABBITIM_USER_LIBCURL
void MainWindow::slotUpdateExec(int nError, const QString &szFile)
{
    CDlgUpdate dlg(nError, szFile, this);
    dlg.exec();
}
#endif
