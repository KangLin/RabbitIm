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
#include "Global/Global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent
#ifndef MOBILE
                , Qt::WindowCloseButtonHint 
                #ifndef DEBUG
                | Qt::WindowStaysOnTopHint
                #endif
#endif
                ),
    m_TrayIcon( this),
    m_TrayIconMenu(this),
    m_ActionGroupStatus(this),
    m_ActionGroupTranslator(this),
    m_ActionGroupStyle(this),
    m_Login(new CFrmLogin(this)),
    m_nLoginFlag(0),
    ui(new Ui::MainWindow)
{
    CGlobal::Instance()->SetMainWindow(this);

#ifdef MOBILE
    CTool::SetWindowsGeometry(this);
#endif
    ui->setupUi(this);
#ifndef MOBILE
    m_bAnimationHide = false;
    m_nHideSize = 5;
    m_nBorderSize = 30;
    m_MainAnimation.hide();
    m_Animation = QSharedPointer<QPropertyAnimation>(
                new QPropertyAnimation(&m_MainAnimation, "geometry"));
#endif
    m_bLogin = false;

    bool check;
    check = connect(ui->actionAbout_A, SIGNAL(triggered()),
            SLOT(slotAbout()));
    Q_ASSERT(check);

    LoadStyle();
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
    
#ifdef ANDROID
    check = connect(&m_AndroidNotify, SIGNAL(sigOnChick()),
                    SLOT(slotMessageClicked()));
    Q_ASSERT(check);
#endif
    
#ifndef MOBILE
    //检测隐藏窗口定时器  
    check = connect(&m_timerAnimation, SIGNAL(timeout()),
                    SLOT(slotCheckHideWindows()));
    Q_ASSERT(check);
    check = connect(&m_MainAnimation, SIGNAL(sigShow()),
                    SLOT(slotCheckShowWindows()));
    Q_ASSERT(check);
    check = connect(m_Animation.data(), SIGNAL(finished()),
                    SLOT(slotAnimationFinished()));
    Q_ASSERT(check);
#endif

    //安装事件监听器  
    //this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::~MainWindow()");
    //TODO:可能会引起程序core  
    //emit sigRemoveMenu(ui->menuOperator_O);

#ifndef MOBILE
    //保存窗口位置  
    QRect rect = this->frameGeometry();
    slotCheckShowWindows();
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                   QSettings::IniFormat);
    conf.setValue("UI/MainWindow/top", this->y());
    conf.setValue("UI/MainWindow/left", this->x());
    conf.setValue("UI/MainWindow/width", this->width());
    conf.setValue("UI/MainWindow/height", this->height());
#endif

    this->ClearTranslate();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    Q_UNUSED(e);
    /*
    LOG_MODEL_DEBUG("MainWindow", "\ngeometry:top:%d;left:%d;right:%d;bottom:%d;\nheight:%d;width:%d;\nframegeometry:top:%d;left:%d;right:%d;bottom:%d",
                    geometry().top(), geometry().left(),
                    geometry().right(), geometry().bottom(),
                    geometry().height(), geometry().width(),
                    frameGeometry().top(), frameGeometry().left(),
                    frameGeometry().right(), frameGeometry().bottom()
                    );//*/

    /*if(!m_TableMain.isNull())
        m_TableMain->resize(this->geometry().size());*/
}

void MainWindow::showEvent(QShowEvent *)
{
#ifndef MOBILE
    slotCheckShowWindows();
#endif
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent");
    if(!m_bLogin)
    {
        LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent QApplication::quit()");
        e->accept(); 
        //QApplication::quit();
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
                        QMessageBox::Ok | QMessageBox::Yes | QMessageBox::Cancel,
                        this);
        msg.setButtonText(QMessageBox::Ok , tr("Close"));
        msg.setButtonText(QMessageBox::Yes, tr("Logout"));
        msg.setButtonText(QMessageBox::Cancel, tr("Cancel"));
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
            slotLogout();
        }
        //else if(msg.clickedButton() == pLogout)
        else if(QMessageBox::Yes == nRet)
        {
            CGlobal::Instance()->SetCloseType(CGlobal::E_CLOSE_TYPE_LOGOUT);
        }
    }
        break;
    case CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME:
    case CGlobal::E_CLOSE_TYPE_LOGOUT:
    default:
        break;
    }
    //先完成注销流程，再关闭程序   
    m_nLoginFlag = 1;//标志退出是从关闭按钮发起  
    slotLogout();
    e->ignore(); //忽略退出事件  
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

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    //LOG_MODEL_DEBUG("MainWindow", "MainWindow::enterEvent");

#ifndef MOBILE
    m_timerAnimation.stop();
    QRect rect;
    this->slotCheckShowWindows();
#endif
}

void MainWindow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    //LOG_MODEL_DEBUG("MainWindow", "MainWindow::leaveEvent");

#ifndef MOBILE
    //启动窗口隐藏检测定时器  
    m_timerAnimation.start(CGlobal::Instance()->GetAnimationHideMainWindow());
    //slotCheckHideWindows();
#endif
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);
    //LOG_MODEL_DEBUG("MainWindow", "MainWindow::moveEvent");
    //this->setWindowOpacity(0.5);

#ifndef MOBILE
    //启动窗口隐藏检测定时器  
    //m_timerAnimation.start(CGlobal::Instance()->GetAnimationHideMainWindow());
#endif
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
#ifdef DEBUG
    /*
    static long s = 0;
    LOG_MODEL_DEBUG("MainWindow", "QEvent type%d:%d", s++, event->type());//*/
#endif
    if (target == this) { 
        if(event->type() == QEvent::Enter)
        {
            LOG_MODEL_DEBUG("MainWindow", "QEvent::Enter");
        }
    }
    return QMainWindow::eventFilter(target, event);
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
        QSharedPointer<CFrmMain> main(new CFrmMain(this));
        m_TableMain = main;
    }
    if(!m_TableMain.isNull())
    {
        this->setCentralWidget(m_TableMain.data());
    }
    else
        LOG_MODEL_ERROR("MainWindow", "new CWdgTableMain fail");

    bool check = connect(this, SIGNAL(sigRefresh()),
                    this, SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);

    slotUpdateLocaleUserInfo();
    m_bLogin = true;
    ReInitMenuOperator();
}

void MainWindow::slotClientDisconnected()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: slotClientDisconnected");
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

    if((CGlobal::Instance()->GetCloseType()
        == CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME)
            && (1 == m_nLoginFlag))
    {
        m_nLoginFlag = 0;
        this->close();
    }
    else
    {
        setWindowTitle(tr("RabbitIm"));
        setWindowIcon(QIcon(":/icon/AppIcon"));
        ReInitMenuOperator();    
    }
}

void MainWindow::slotUpdateLocaleUserInfo()
{
    QSharedPointer<CUser> user = USER_INFO_LOCALE;
    if(user.isNull())
    {
        LOG_MODEL_ERROR("MainWindow", "Locale is null");
        return;
    }
    QSharedPointer<CUserInfo> info = user->GetInfo();
    if(info.isNull())
    {
        LOG_MODEL_ERROR("MainWindow", "info is null");
        return;
    }
    user->GetInfo()->SetStatus(CGlobal::Instance()->GetStatus());
    this->m_TrayIcon.setToolTip(tr("RabbitIm:%1").arg(
                   GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName()));
    this->setWindowTitle(tr("RabbitIm:%1").arg(
                   GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName()));
    QPixmap pmp;
    ComposeAvatarStatus(user->GetInfo(), pmp);
    setWindowIcon(pmp);
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

    emit sigRefresh();
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
    QAction* pAct = m_ActionStyles[CGlobal::Instance()->GetStyleMenu()];
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

    //从登录界面保存的全局变量中得到当前用户的状态  
    CUserInfo::USER_INFO_STATUS status = CGlobal::Instance()->GetStatus();
    //USER_INFO_LOCALE->GetInfo()->SetStatus(status);//第一次启动时,对象非生成,在slotUpdateLocaleUserInfo中初始化  
    QAction* pAct = m_ActionStatus[status];
    if(pAct)
    {
        pAct->setChecked(true);
    }

    m_MenuStatus.setTitle(tr("Status(&S)"));
    m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(status)));

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
    m_ActionTranslator["Default"] = m_MenuTranslate.addAction(
                QIcon(":/icon/Language"), tr("Default"));
    m_ActionTranslator["English"] = m_MenuTranslate.addAction(
                QIcon(":/icon/English"), tr("English"));
    m_ActionTranslator["zh_CN"] = m_MenuTranslate.addAction(
                QIcon(":/icon/China"), tr("Chinese"));
    m_ActionTranslator["zh_TW"] = m_MenuTranslate.addAction(
                QIcon(":/icon/China"), tr("Chinese(TaiWan)"));

    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupTranslator.addAction(it.value());
    }

    LOG_MODEL_DEBUG("MainWindow",
                    "MainWindow::InitMenuTranslate m_ActionTranslator size:%d",
                    m_ActionTranslator.size());

    bool check = connect(&m_ActionGroupTranslator, SIGNAL(triggered(QAction*)),
                        SLOT(slotActionGroupTranslateTriggered(QAction*)));
    Q_ASSERT(check);

    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                   QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", "Default").toString();
    QAction* pAct = m_ActionTranslator[szLocale];
    if(pAct)
    {
        LOG_MODEL_DEBUG("MainWindow",
                        "MainWindow::InitMenuTranslate setchecked locale:%s",
                        szLocale.toStdString().c_str());
        pAct->setChecked(true);
        LOG_MODEL_DEBUG("MainWindow",
                        "MainWindow::InitMenuTranslate setchecked end");
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

    LOG_MODEL_DEBUG("MainWindow",
                    "MainWindow::ClearMenuTranslate m_ActionTranslator size:%d",
                    m_ActionTranslator.size());
    
    return 0;
}

int MainWindow::ClearTranslate()
{
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
    return 0;
}

int MainWindow::LoadTranslate(QString szLocale)
{
    //初始化翻译  
    if(szLocale.isEmpty())
    {
        QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                       QSettings::IniFormat);
        szLocale = conf.value("Global/Language",
                              QLocale::system().name()).toString();
    }

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    LOG_MODEL_DEBUG("main", "locale language:%s",
                    szLocale.toStdString().c_str());

    ClearTranslate();
    LOG_MODEL_DEBUG("MainWindow", "Translate dir:%s",
                    qPrintable(CGlobalDir::Instance()->GetDirTranslate()));

    m_TranslatorQt = QSharedPointer<QTranslator>(new QTranslator(this));
    m_TranslatorQt->load("qt_" + szLocale + ".qm",
                         CGlobalDir::Instance()->GetDirTranslate());
    qApp->installTranslator(m_TranslatorQt.data());

    m_TranslatorApp = QSharedPointer<QTranslator>(new QTranslator(this));
#ifdef ANDROID
    m_TranslatorApp->load(":/translations/app_" + szLocale + ".qm");
#else
    m_TranslatorApp->load("app_" + szLocale + ".qm",
                          CGlobalDir::Instance()->GetDirTranslate());
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
            QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                           QSettings::IniFormat);
            conf.setValue("Global/Language", szLocale);
            LOG_MODEL_DEBUG("MainWindow",
                            "MainWindow::slotActionGroupTranslateTriggered:%s",
                            it.key().toStdString().c_str());
            LoadTranslate(it.key());
            pAct->setChecked(true);
            QMessageBox::information(this, tr("Information"),
                                     tr("Change language must reset program."));
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
#ifdef ANDROID
        m_AndroidNotify.setNotification(szMessage, szTitle);
#else
        m_TrayIcon.showMessage(szTitle,
                               szMessage, 
                               QSystemTrayIcon::Information,
                               CGlobal::Instance()->GetNotifiationBarShowMessageDelay());
#endif
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
    if(this->isHidden() || this->isMinimized()
        #ifndef MOBILE
            || m_bAnimationHide
        #endif
            )
        szTitle = tr("Show Main Windows");
    else
        szTitle = tr("Hide Main Windows");

    m_TrayIconMenu.addAction(QIcon(":/icon/AppIcon"), szTitle, this,
                      SLOT(on_actionNotifiation_show_main_windows_triggered()));

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
    if(isHidden() || isMinimized()
        #ifndef MOBILE
            || m_bAnimationHide
        #endif
            )
     {
#ifndef MOBILE
        QRect rect;
        this->slotCheckShowWindows();
#endif
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
            CGlobal::Instance()->SetStatus(status);
            GET_CLIENT->setClientStatus(status);
            act->setChecked(true);
            m_MenuStatus.setIcon(QIcon(CGlobal::Instance()->GetRosterStatusIcon(status)));
            emit sigRefresh();
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

void MainWindow::on_actionScan_qrencode_S_triggered()
{
    CPluginApp* plugin = GETMANAGER->GetManagePluginApp()->GetPlugin("QRCode");
    if(NULL == plugin)
        return;
    plugin->Open(this);
}

void MainWindow::slotAbout()
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
                CGlobal::Instance()->SetStyleMenu("Blue", ":/sink/Blue");
            else if(it.key() == "Dark")
                CGlobal::Instance()->SetStyleMenu("Dark", ":/qdarkstyle/style.qss");
            else if(it.key() == "Custom")
                OpenCustomStyleMenu();
            else
                CGlobal::Instance()->SetStyleMenu("System", "");
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
                        CGlobal::Instance()->GetStyle().toStdString().c_str(),
                        file.error());
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
        QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                       QSettings::IniFormat);
        conf.setValue("UI/StyleSheet", szFile);
        
        CGlobal::Instance()->SetStyleMenu("Custom", szFile);
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d", 
                        szFile.toStdString().c_str(), file.error());
    }
    return 0;
}

#ifndef MOBILE
int MainWindow::AnimationWindows(const QRect &startRect, const QRect &endRect)
{
    if(m_Animation->state() != QAbstractAnimation::Stopped)
    {
        LOG_MODEL_DEBUG("MainWindow", "animation is run");
        return -1;//m_Animation->stop();
    }
    m_Animation->setDuration(CGlobal::Instance()->GetAnimationDuration());
    m_Animation->setStartValue(startRect);
    m_Animation->setEndValue(endRect);
    m_Animation->start();
    return 0;
}

void MainWindow::slotCheckHideWindows()
{
    m_timerAnimation.stop();

    if(!m_bLogin)
        return;

    if(!CGlobal::Instance()->IsAnimationHideMainWindow())
    {
        return;
    }

    if(m_Animation->state() != QAbstractAnimation::Stopped)
    {
        LOG_MODEL_DEBUG("MainWindow", "animation is run");
        return;
    }

    if(m_bAnimationHide)
        return;

    //保存窗口大小  
    QRect startRect, endRect;
    startRect = this->frameGeometry();
    endRect = startRect;
    if(this->frameGeometry().top() < m_nBorderSize)//向上边隐藏  
    {
        endRect.setHeight(m_nHideSize);
        endRect.moveTop(0);
        if(this->frameGeometry().left() < 0)
        {
            endRect.moveLeft(0);
        }
        else if(this->frameGeometry().right() > QApplication::desktop()->width())
        {
            endRect.moveRight(QApplication::desktop()->width());
        }
    }
    else if(this->frameGeometry().bottom() > QApplication::desktop()->height()
            - m_nBorderSize)//向下边隐藏  
    {
        endRect.setHeight(m_nHideSize);
        endRect.moveBottom(QApplication::desktop()->height());
        if(this->frameGeometry().left() < 0)
        {
            endRect.moveLeft(0);
        }
        else if(this->frameGeometry().right() > QApplication::desktop()->width())
        {
            endRect.moveRight(QApplication::desktop()->width());
        }
    }
    else if(this->frameGeometry().left() < m_nBorderSize)//向左边隐藏  
    {
        endRect.setWidth(m_nHideSize);
        endRect.moveLeft(0);        
        if(this->frameGeometry().top() < 0)
        {
            endRect.moveTop(0);
        }
        else if(this->frameGeometry().bottom() > QApplication::desktop()->height())
        {
            endRect.moveBottom(QApplication::desktop()->height());
        }
    }
    else if(this->frameGeometry().right() > QApplication::desktop()->width()
            - m_nBorderSize)//向右边隐藏  
    {
        endRect.setWidth(m_nHideSize);
        endRect.moveRight(QApplication::desktop()->width());
        if(this->frameGeometry().top() < 0)
        {
            endRect.moveTop(0);
        }
        else if(this->frameGeometry().bottom() > QApplication::desktop()->height())
        {
            endRect.moveBottom(QApplication::desktop()->height());
        }
    }
    else
        return;

    m_bAnimationHide = true;
    m_MainAnimation.setGeometry(startRect);
    m_MainAnimation.show();
    this->hide();
    if(AnimationWindows(startRect, endRect))
        return;
}

void MainWindow::slotCheckShowWindows()
{
    if(!m_bAnimationHide)
        return;

    if(m_Animation->state() != QAbstractAnimation::Stopped)
    {
        LOG_MODEL_DEBUG("MainWindow", "animation is run");
        return;//m_Animation->stop();
    }

    QRect startRect = m_MainAnimation.frameGeometry();
    QRect endRect = this->frameGeometry();
    if(m_MainAnimation.frameGeometry().height() <= m_nHideSize)
    {
        if(m_MainAnimation.frameGeometry().bottom() 
                < QApplication::desktop()->height() >> 2)//向上边隐藏  
        {
            endRect.moveTop(0);
        }
        else if(m_MainAnimation.frameGeometry().top()
                > QApplication::desktop()->height() >> 2)//向下边隐藏  
        {
            endRect.moveBottom(QApplication::desktop()->height());           
        }
        if(this->frameGeometry().left() < 0)
            endRect.moveLeft(0);
        else if(this->frameGeometry().right() > QApplication::desktop()->width())
            endRect.moveRight(QApplication::desktop()->width());
    }
    else if(m_MainAnimation.frameGeometry().width() <= m_nHideSize)
    {
        if(m_MainAnimation.frameGeometry().left()
                >= QApplication::desktop()->width() >> 2) //向右边隐藏  
        {
            endRect.moveRight(QApplication::desktop()->width());
        }
        else if(m_MainAnimation.frameGeometry().right()
                <= QApplication::desktop()->width() >> 2) //向左边隐藏  
        {
            endRect.moveLeft(0);
        }
        if(this->frameGeometry().top() < 0)
            endRect.moveTop(0);
        else if(this->frameGeometry().bottom() > QApplication::desktop()->height())
            endRect.moveBottom(QApplication::desktop()->height());
    }
    else
    {
        LOG_MODEL_ERROR("MainWindow", "CheckShowWindows error");
        return;
    }

    this->move(endRect.x(), endRect.y());
    m_bAnimationHide = false;
    if(AnimationWindows(startRect, endRect))
        return;

    return;
}

void MainWindow::slotAnimationFinished()
{
    if(m_bAnimationHide)
        return;

    this->show();
    m_MainAnimation.hide();
    this->activateWindow();
}

#endif

int MainWindow::ComposeAvatarStatus(QSharedPointer<CUserInfo> info, QPixmap &outPixmap)
{
    if(info.isNull())
        return -1;
    //转换成灰度图像  
    if(info->GetStatus() == CUserInfo::OffLine 
       || info->GetStatus() == CUserInfo::Invisible)
    {
        QIcon icon(info->GetPhotoPixmap());
        outPixmap = icon.pixmap(info->GetPhotoPixmap().width(), 
                                info->GetPhotoPixmap().height(), 
                                QIcon::Disabled);
        //outPixmap = QPixmap::fromImage(info->GetPhoto().convertToFormat(QImage::Format_Mono));
        return 0;
    }
    outPixmap = info->GetPhotoPixmap();
    QPixmap pStatus(CGlobal::Instance()->GetRosterStatusIcon(info->GetStatus()));
    CTool::ComposeAvatarStatus(outPixmap, pStatus);
    return 0;
}
