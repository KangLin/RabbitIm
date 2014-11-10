#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <QIcon>
#include "Widgets/FrmAbout/FrmAbout.h"
#include <QMessageBox>
#include "Widgets/FrmUserList/FrmUserList.h"
#include "Widgets/FrmLogin/FrmLogin.h"
#include "Global/Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Widgets/DlgOptions/DlgOptions.h"
#include "Widgets/FrmSendFile/DlgSendManage.h"
#include "Widgets/FrmUservCard/FrmUservCard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_TrayIcon( this),
    m_TrayIconMenu(this),
    m_ActionGroupStatus(this),
    m_ActionGroupTranslator(this),
    m_Login(new CFrmLogin(this)),
    ui(new Ui::MainWindow)
{
    CGlobal::Instance()->SetMainWindow(this);

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

        //TODO:
        //CFrmVideo::instance();
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
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());

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
    case 0:
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Close"),
                        tr("Is close the programe or logout?"),
                        QMessageBox::Ok | QMessageBox::Yes | QMessageBox::Cancel);
        msg.setButtonText(QMessageBox::Ok , tr("close"));
        msg.setButtonText(QMessageBox::Yes, tr("logout"));
        msg.setDefaultButton(QMessageBox::Ok);
        int nRet = msg.exec();
        if(QMessageBox::Ok == nRet)
        {
            CGlobal::Instance()->SetCloseType(CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME);
            //退出程序  
            GET_CLIENT->Logout();
            e->accept();
            QApplication::closeAllWindows();
        }
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
    case 1:
        //退出程序  
        GET_CLIENT->Logout();
        e->accept();
        QApplication::closeAllWindows();
        break;
    case 2:
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
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::changeEvent.e->type:%d", e->type());
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

    InitOperatorMenu();
    return 0;
}

int MainWindow::InitOperatorMenu()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::InitOperatorMenu");
    ui->menuOperator_O->addAction(QIcon(":/icon/Stype"), tr("Change Style Sheet(&S)"), 
                this, SLOT(on_actionChange_Style_Sheet_S_triggered()));
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
        m_ActionGroupTranslator.addAction(it.value());

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
    m_TranslatorApp->load("app_" + szLocale + ".qm", CGlobal::Instance()->GetDirTranslate());
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
            act->setCheckable(true);
            act->setChecked(true);
            m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(status)));
        }
    }
}

void MainWindow::slotEditInformation()
{
    CFrmUservCard* pvCard = 
            new CFrmUservCard(USER_INFO_LOCALE->GetInfo(), true);
    pvCard->show();
    pvCard->activateWindow();
}

void MainWindow::slotLogout()
{
    GET_CLIENT->Logout();
}
/*
void MainWindow::onReceiveFile(QXmppTransferJob *job)
{
    if(job)
    {
        m_pSendManageDlg->addFileProcess(*job,false);
    }
}*/

void MainWindow::on_actionOptions_O_triggered()
{
    CDlgOptions dlg(this);
    bool check = connect(&dlg, SIGNAL(sigRefresh()), SIGNAL(sigRefresh()));
    Q_ASSERT(check);

    dlg.exec();
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
#ifdef MOBILE
    QDesktopWidget *pDesk = QApplication::desktop();
    QFileDialog dlg(pDesk, tr("Open File"), QString(), "*.qss");
    //dlg.setGeometry(this->rect());
    QScreen* pScreen = QApplication::primaryScreen();
    LOG_MODEL_DEBUG("MainWindow", "DeskWidth:%d;height:%d;w:%d;h:%d;screenWidth:%d;height:%d;w%d;h%d", 
                    pDesk->geometry().width(),
                    pDesk->geometry().height(),
                    pDesk->availableGeometry().width(),
                    pDesk->availableGeometry().height(),
                    pScreen->geometry().width(),
                    pScreen->geometry().height(),
                    pScreen->availableGeometry().width(),
                    pScreen->availableGeometry().height()
                    );
    dlg.setGeometry(pScreen->availableGeometry());
    QString szFile;
    QStringList fileNames;
    if(dlg.exec())
        fileNames = dlg.selectedFiles();
    else
        return;
    if(fileNames.isEmpty())
        return;
    szFile = *fileNames.begin();
#else
    QString szFile = QFileDialog::getOpenFileName(
                this, tr("Open File"), 
                QString(), "*.qss", 0,
                QFileDialog::ReadOnly | QFileDialog::DontUseNativeDialog);
#endif

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
    }
}
