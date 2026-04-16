#include <iostream>
#include <QIcon>
#include <QLoggingCategory>
#include <QMessageBox>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QScreen>
#endif
#include "RabbitCommonDir.h"
#include "RabbitCommonTools.h"
#include "FrmUpdater/FrmUpdater.h"
#include "DlgAbout/DlgAbout.h"
#include "Widgets/FrmUserList/FrmUserList.h"
#include "Widgets/FrmLogin/FrmLogin.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Widgets/DlgOptions/DlgOptions.h"
//#include "Widgets/FrmSendFile/DlgSendManage.h"
#include "Widgets/DlgUservCard/DlgUservCard.h"

#include "Global/Global.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

static Q_LOGGING_CATEGORY(log, "windows")
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent
#ifndef MOBILE
                , Qt::WindowCloseButtonHint 
//                #ifndef DEBUG
//                | Qt::WindowStaysOnTopHint
//                #endif
#endif
                ),
    ui(new Ui::MainWindow),
    m_Login(new CFrmLogin(this)),
    m_nLoginFlag(0),
    m_TrayIconMenu(this),
    m_TrayIcon(this),
    m_ActionGroupStatus(this)
{
    CGlobal::Instance()->SetMainWindow(this);

#ifdef MOBILE
    CTool::SetWindowsGeometry(this);
#endif
    ui->setupUi(this);
#ifndef MOBILE

    m_Translator = RabbitCommon::CTools::Instance()->InstallTranslator(
        "RabbitIm", RabbitCommon::CTools::TranslationType::Library);

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
#ifdef RABBITCOMMON  
    CFrmUpdater updater;
    ui->actionUpdate->setIcon(updater.windowIcon());
#endif

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
    check = connect(&m_AndroidNotify, SIGNAL(sigOnChilk()),
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
    qDebug(log) << Q_FUNC_INFO;
    //TODO:可能会引起程序core  
    //emit sigRemoveMenu(ui->menuOperator_O);

#ifndef MOBILE
    //保存窗口位置  
    //QRect rect = this->frameGeometry();
    slotCheckShowWindows();
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("UI/MainWindow/top", this->y());
    conf.setValue("UI/MainWindow/left", this->x());
    conf.setValue("UI/MainWindow/width", this->width());
    conf.setValue("UI/MainWindow/height", this->height());
#endif

    this->ClearMenuStatus();
    delete ui;

    if(m_Translator)
        RabbitCommon::CTools::Instance()->RemoveTranslator(m_Translator);
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    Q_UNUSED(e)
    /*
    qDebug("\ngeometry:top:%d;left:%d;right:%d;bottom:%d;\nheight:%d;width:%d;\nframegeometry:top:%d;left:%d;right:%d;bottom:%d",
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
    qDebug(log) << Q_FUNC_INFO;
    if(!m_bLogin)
    {
        qDebug(log) << "MainWindow::closeEvent QApplication::quit()";
        e->accept(); 
        //QApplication::quit();
        return;
    }

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
    //qDebug(log) << Q_FUNC_INFO << e;
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void MainWindow::enterEvent(QEnterEvent *event)
#else
void MainWindow::enterEvent(QEvent* event)
#endif
{
    Q_UNUSED(event)
    qDebug(log) << Q_FUNC_INFO;

#ifndef MOBILE
    m_timerAnimation.stop();
    this->slotCheckShowWindows();
#endif
}

void MainWindow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    qDebug(log) << Q_FUNC_INFO;

#ifndef MOBILE
    //启动窗口隐藏检测定时器  
    m_timerAnimation.start(CGlobal::Instance()->GetAnimationHideMainWindow());
    //slotCheckHideWindows();
#endif
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event)
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
            qDebug(log) << "QEvent::Enter";
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
        qCritical() << "new CWdgTableMain fail";

    bool check = connect(this, SIGNAL(sigRefresh()),
                    this, SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);

    slotUpdateLocaleUserInfo();
    m_bLogin = true;
    ReInitMenuOperator();
}

void MainWindow::slotClientDisconnected()
{
    qDebug(log) << Q_FUNC_INFO;
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
        qCritical() << "Locale is null";
        return;
    }
    QSharedPointer<CUserInfo> info = user->GetInfo();
    if(info.isNull())
    {
        qCritical() << "info is null";
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
                tr("Edit Locale User Information(&E)"),
                this, SLOT(slotEditInformation()));
    //注册菜单  
    emit sigMenuInitOperator(ui->menuOperator_O);
 
    ui->menuOperator_O->addSeparator();

    InitOperatorMenu();
    return 0;
}

int MainWindow::InitOperatorMenu()
{
    qDebug(log) << Q_FUNC_INFO;
    RabbitCommon::CTools::AddStyleMenu(ui->menuOperator_O);
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

void MainWindow::slotTrayIconActive(QSystemTrayIcon::ActivationReason e)
{
    qDebug(log) << Q_FUNC_INFO << e;
    if(QSystemTrayIcon::Trigger == e)
    {
        slotMessageClicked();
        this->show();
        this->activateWindow();
    }
}

void MainWindow::slotMessageClicked()
{
    qDebug(log) << Q_FUNC_INFO;
    CGlobal::Instance()->GetManager()->GetRecentMessage()->ShowLastMessageDialog();
}

//在通知栏上显示消息  
int MainWindow::ShowTrayIconMessage(const QString &szTitle, const QString &szMessage)
{
    if(CGlobal::Instance()->IsNotifiationBarShowMessage())
    {
#ifdef ANDROID
        m_AndroidNotify.Show(szMessage, szTitle);
#else
        m_TrayIcon.showMessage(szTitle,
                               szMessage, 
                               QSystemTrayIcon::Information,
                               CGlobal::Instance()->GetNotifiationBarShowMessageDelay());
#endif
    }
    if(CGlobal::Instance()->IsNotifiationFlashs())
        slotTrayTimerStart();
    
    if(CGlobal::Instance()->GetMessageSound()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QSoundEffect effect;
        effect.setSource(QUrl::fromLocalFile(":/sound/Message"));
        effect.play();
#else
        QSound::play(":/sound/Message");
#endif
    }

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
                    tr("Edit Locale User Information(&E)"),
                    this, SLOT(slotEditInformation()));
    }

    m_TrayIconMenu.addAction(ui->actionOptions_O);

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
    if(nullptr == plugin)
        return;
    plugin->Open(this);
}

void MainWindow::slotAbout()
{
#ifdef HAVE_ABOUT
        CDlgAbout *about = new CDlgAbout(this);
        about->m_AppIcon = QImage(":/icon/AppIcon");
        about->m_szCopyrightStartTime = "2013";
        about->m_szVersionRevision = RabbitIm_REVISION;
        RC_SHOW_WINDOW(about);
#endif
}

void MainWindow::on_actionUpdate_triggered()
{
#ifdef RABBITCOMMON
    CFrmUpdater* pfrmUpdater = new CFrmUpdater();
    pfrmUpdater->SetTitle(QImage(":/icon/AppIcon"));
    #if defined (Q_OS_ANDROID)
        pfrmUpdater->showMaximized();
    #else
        pfrmUpdater->show();
    #endif
#endif
}

#ifndef MOBILE
int MainWindow::AnimationWindows(const QRect &startRect, const QRect &endRect)
{
    if(m_Animation->state() != QAbstractAnimation::Stopped)
    {
        qDebug(log) << "animation is run";
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
        qDebug(log) << "animation is run";
        return;
    }

    if(m_bAnimationHide)
        return;

    //保存窗口大小  
    QRect startRect, endRect;
    startRect = this->frameGeometry();
    endRect = startRect;
    QSize desktopSize = QApplication::primaryScreen()->availableGeometry().size();
    int desktopWidth = desktopSize.width();
    int desktopHeight = desktopSize.height();
    if(this->frameGeometry().top() < m_nBorderSize)//向上边隐藏  
    {
        endRect.setHeight(m_nHideSize);
        endRect.moveTop(0);
        if(this->frameGeometry().left() < 0)
        {
            endRect.moveLeft(0);
        }
        else if(this->frameGeometry().right() > desktopWidth)
        {
            endRect.moveRight(desktopWidth);
        }
    }
    else if(this->frameGeometry().bottom() > desktopHeight
            - m_nBorderSize)//向下边隐藏  
    {
        endRect.setHeight(m_nHideSize);
        endRect.moveBottom(desktopHeight);
        if(this->frameGeometry().left() < 0)
        {
            endRect.moveLeft(0);
        }
        else if(this->frameGeometry().right() > desktopWidth)
        {
            endRect.moveRight(desktopWidth);
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
        else if(this->frameGeometry().bottom() > desktopHeight)
        {
            endRect.moveBottom(desktopHeight);
        }
    }
    else if(this->frameGeometry().right() > desktopWidth
            - m_nBorderSize)//向右边隐藏  
    {
        endRect.setWidth(m_nHideSize);
        endRect.moveRight(desktopWidth);
        if(this->frameGeometry().top() < 0)
        {
            endRect.moveTop(0);
        }
        else if(this->frameGeometry().bottom() > desktopHeight)
        {
            endRect.moveBottom(desktopHeight);
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
        qDebug(log) << "animation is run";
        return;//m_Animation->stop();
    }

    QScreen* pDesk = QApplication::primaryScreen();
    QRect rDesk = pDesk->geometry();

    QSize desktopSize = QApplication::primaryScreen()->availableGeometry().size();
    int desktopHeight = desktopSize.height();
    QRect startRect = m_MainAnimation.frameGeometry();
    QRect endRect = this->frameGeometry();
    if(m_MainAnimation.frameGeometry().height() <= m_nHideSize)
    {
        if(m_MainAnimation.frameGeometry().bottom() 
                < desktopHeight >> 2)//向上边隐藏  
        {
            endRect.moveTop(0);
        }
        else if(m_MainAnimation.frameGeometry().top()
                > desktopHeight >> 2)//向下边隐藏  
        {
            endRect.moveBottom(desktopHeight);           
        }
        if(this->frameGeometry().left() < 0)
            endRect.moveLeft(0);
        else if(this->frameGeometry().right() > rDesk.width())
            endRect.moveRight(rDesk.width());
    }
    else if(m_MainAnimation.frameGeometry().width() <= m_nHideSize)
    {
        if(m_MainAnimation.frameGeometry().left()
                >= rDesk.width() >> 2) //向右边隐藏
        {
            endRect.moveRight(rDesk.width());
        }
        else if(m_MainAnimation.frameGeometry().right()
                <= rDesk.width() >> 2) //向左边隐藏
        {
            endRect.moveLeft(0);
        }
        if(this->frameGeometry().top() < 0)
            endRect.moveTop(0);
        else if(this->frameGeometry().bottom() > desktopHeight)
            endRect.moveBottom(desktopHeight);
    }
    else
    {
        qCritical("CheckShowWindows error");
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
    CTool::MergeAvatarStatus(outPixmap, pStatus);
    return 0;
}
