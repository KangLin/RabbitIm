#include "Global.h"
#include "common/Tool.h"
#include "../MainWindow.h"
#include "Media/Camera/CameraFactory.h"
#include "GlobalDir.h"
#include <QMetaType>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <string>
#include <QSettings>
#include <QAudioDeviceInfo>

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    m_pMainWindow = NULL;
    m_pManager = NULL;

    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    m_LocalStatus = (CUserInfo::USER_INFO_STATUS)conf.value(
                "Login/LoginState", CUserInfo::Online).toInt();
    m_UserColor = GetColorFormConf("Options/User/LocalColor",
                                   QColor(255, 0, 0));
    m_RosterColor = GetColorFormConf("Options/User/RosterColor",
                                     QColor(0, 0, 255));
    m_UserMessageColor = GetColorFormConf("Options/User/LocalMessageColor",
                                          QColor(0, 0, 0));
    m_RosterMessageColor = GetColorFormConf("Options/User/RosterMessageColor",
                                            QColor(0, 0, 0));
    m_UnreadMessageCountColor = GetColorFormConf(
                "Options/User/UnreadMessageCountColor", QColor(255, 0, 0));

    m_CloseType =  (E_CLOSE_TYPE)conf.value("Logout/type",
                                            E_CLOSE_TYPE_NO).toInt();
#ifdef MOBILE
    m_MessageSendType = (E_MESSAGE_SEND_TYPE)conf.value(
                "Options/Message/SendType", E_MESSAGE_SEND_TYPE_ENTER).toInt();
#else
    m_MessageSendType = (E_MESSAGE_SEND_TYPE)conf.value(
                "Options/Message/SendType",
                E_MESSAGE_SEND_TYPE_CTRL_ENTER).toInt();
#endif

    m_szXmppDomain = conf.value("Login/XmppDomain", "rabbitim.com").toString();
    //m_szXmppServer = conf.value("Login/XmppServer", "rabbitim.wicp.net").toString();
    m_szXmppServer = conf.value("Login/XmppServer", "rabbitim.wicp.net").toString();
    m_szXmppServerPort = conf.value("Login/XmppServerPort", 5222).toInt();
    //m_szStunServer = conf.value("Login/StunServer", "stun.l.google.com").toString();
    m_szStunServer = conf.value("Login/StunServer", "rabbitim.wicp.net").toString();
    //m_szTurnServer = conf.value("Login/TurnServer", "turn.l.google.com").toString();
    m_szTurnServer = conf.value("Login/TurnServer", "rabbitim.wicp.net").toString();
    m_nStunServerPort = conf.value("Login/StunServerPort", 3478).toInt();
    m_nTurnServerPort = conf.value("Login/TurnServerPort", 3478).toInt();
    m_szTurnUser = conf.value("Login/TurnServerUser", "1").toString();
    m_szTurnPassword = conf.value("Login/TurnServerPassword", "1").toString();

    m_AutoLogin = conf.value("Login/AutoLogin", false).toBool();
    m_nAutoLoginDelayTime = conf.value("Login/AutoLoginDelayTime", "3").toInt();
    m_bNotifiationBarShowMessage = conf.value(
                "Options/NotifiationBar/ShowMessage", true).toBool();
    m_nShowMessageDelay = conf.value(
                "Options/NotifiationBar/ShowMessageDelay", 10000).toInt();
    m_bNotifiationBarFlashs =  conf.value(
                "Options/NotifiationBar/Flashs", true).toBool();
    m_nFlashInterval = conf.value(
                "Options/NotifiationBar/FlashsInterval", 500).toInt();
    m_bMessageSound = conf.value(
                "Options/Message/ReceiveMessageSound", true).toBool();
    m_nAnimationHideMainWindow = conf.value(
                "UI/MainWindow/AnimationHide", 3000).toInt();
    m_nAnimationDuration = conf.value(
                "UI/MainWindow/AnimationDuration", 250).toInt();
    m_bAnimationHideMainWindows = conf.value(
                "UI/MainWindow/AutoHide", true).toBool();

    m_RosterShowType = (E_ROSTER_SHOW_TYPE)conf.value(
                "Options/Roster/ShowType", E_ROSTER_SHOW_NAME).toInt();
    m_ScreenShotToType = (E_SCREEN_SHOT_TO_TYPE)conf.value(
                "Options/ScreenShot/ToType", E_TO_SEND).toInt();
    m_bHideMessageBox = conf.value(
                "Options/ScreenShot/HideMessageBox", false).toBool();

    m_Update = (E_UPDATE)conf.value(
                "Options/Update", E_UPDATE_EVERY_TIME).toInt();

    std::vector<CCameraInfo::CamerInfo> info;
    CCameraFactory::Instance()->EnumDevice(info);
    if(info.size() > 0)
        m_nVideoCaptureDevice = conf.value("Device/Video/Capture", 0).toInt();
    else
        m_nVideoCaptureDevice = conf.value("Device/Video/Capture", -1).toInt();
    if(QAudioDeviceInfo::availableDevices(QAudio::AudioInput).size() > 0)
        m_nAudioInputDevice = conf.value("Device/Audio/Input", 0).toInt();
    else
        m_nAudioInputDevice = conf.value("Device/Audio/Input", -1).toInt();
    if(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput).size() > 0)
        m_nAudioOutputDevice = conf.value("Device/Audio/Output", 0).toInt();
    else
        m_nAudioOutputDevice = conf.value("Device/Audio/Output", -1).toInt();

    m_bShowLocaleVideo = conf.value("Video/Locale/Show", true).toBool();
    m_bMonitor = conf.value("Video/Monitor", false).toBool();

    m_szStyleFile = conf.value(
                "UI/StyleSheet", ":/qdarkstyle/style.qss").toString();
    m_szStyleMenu = conf.value("UI/MenuStyleSheet", "Dark").toString();

    //如果不同线程间信号发送中的参数有自定义的数据类型，  
    //那么就必须先注册到Qt内部的类型管理器中后才能在connect()中使用  
#ifdef RABBITIM_USE_QXMPP
    qRegisterMetaType<QXmppVideoFrame>("QXmppVideoFrame");
#endif
}

CGlobal::~CGlobal()
{
}

CGlobal* CGlobal::Instance()
{
    static CGlobal* p = NULL;
    if(!p)
        p = new CGlobal;
    return p;
}

MainWindow* CGlobal::GetMainWindow()
{
    Q_ASSERT(m_pMainWindow);
    return m_pMainWindow;
}

int CGlobal::SetMainWindow(MainWindow *pWnd)
{
    m_pMainWindow = pWnd;
    return 0;
}

CManager *CGlobal::GetManager()
{
    Q_ASSERT(m_pManager);
    return m_pManager;
}

int CGlobal::SetManager(CManager *pManager)
{
    m_pManager = pManager;
    return 0;
}

CUserInfo::USER_INFO_STATUS CGlobal::GetStatus()
{
    return m_LocalStatus;
}

int CGlobal::SetStatus(CUserInfo::USER_INFO_STATUS status)
{
    m_LocalStatus = status;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                   QSettings::IniFormat);
    conf.setValue("Login/LoginState", status);
    return 0;
}

QColor CGlobal::GetColorFormConf(const QString &Key, const QColor &def)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    return QColor(conf.value(Key + "R", def.red()).toInt(),
           conf.value(Key + "G", def.green()).toInt(),
           conf.value(Key + "B", def.blue()).toInt());
}

int CGlobal::SetColorToConf(const QString &Key, const QColor &color)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue(Key + "R", color.red());
    conf.setValue(Key + "G", color.green());
    conf.setValue(Key + "B", color.blue());
    return 0;
}

QColor CGlobal::GetUserColor()
{
    return m_UserColor;
}

int CGlobal::SetUserColor(const QColor &color)
{
    SetColorToConf("Options/User/LocalColor", color);
    m_UserColor = color;
    return 0;
}

QColor CGlobal::GetRosterColor() 
{
    return m_RosterColor;
}

int CGlobal::SetRosterColor(const QColor &color)
{
    SetColorToConf("Options/User/RosterColor", color);
    m_RosterColor = color;
    return 0;
}

QColor CGlobal::GetUserMessageColor()
{
    return m_UserMessageColor;
}

int CGlobal::SetUserMessageColor(const QColor &color)
{
    SetColorToConf("Options/User/LocalMessageColor", color);
    m_UserMessageColor = color;
    return 0;
}

QColor CGlobal::GetRosterMessageColor()
{
    return m_RosterMessageColor;
}

int CGlobal::SetRosterMessageColor(const QColor &color)
{
    SetColorToConf("Options/User/RosterMessageColor", color);
    m_RosterMessageColor = color;
    return 0;
}

QColor CGlobal::GetUnreadMessageCountColor()
{
    return m_UnreadMessageCountColor;
}

int CGlobal::SetUnreadMessageCountColor(const QColor &color)
{
    SetColorToConf("Options/User/UnreadMessageCountColor", color);
    m_UnreadMessageCountColor = color;
    return 0;
}

QString CGlobal::GetXmppDomain()
{
    return m_szXmppDomain;
}

int CGlobal::SetXmppDomain(QString server)
{
    m_szXmppDomain = server;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/XmppDomain", server);
    return 0;
}

QString CGlobal::GetXmppServer()
{
    return m_szXmppServer;
}

int CGlobal::SetXmppServer(QString host)
{
    m_szXmppServer = host;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/XmppServer", host);
    return 0;
}

qint16 CGlobal::GetXmppServerPort()
{
    return m_szXmppServerPort;
}

int CGlobal::SetXmppServerPort(qint16 port)
{
    m_szXmppServerPort = port;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/XmppServerPort", port);
    return 0;
}

QString CGlobal::GetStunServer()
{
    return m_szStunServer;
}

int CGlobal::SetStunServer(QString server)
{
    m_szStunServer = server;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/StunServer", server);
    return 0;
}

qint16 CGlobal::GetStunServerPort()
{
    return m_nStunServerPort;
}

int CGlobal::SetStunServerPort(qint16 port)
{
    m_nStunServerPort = port;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/StunServerPort", port);
    return 0;
}

QString CGlobal::GetTurnServer()
{
    return m_szTurnServer;
}

int CGlobal::SetTurnServer(QString server)
{
    m_szTurnServer = server;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/TurnServer", server);
    return 0;
}

qint16 CGlobal::GetTurnServerPort()
{
    return m_nTurnServerPort;
}

int CGlobal::SetTurnServerPort(qint16 port)
{
    m_nTurnServerPort = port;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/TurnServerPort", port);
    return 0;
}

QString CGlobal::GetTurnServerUser()
{
    return m_szTurnUser;
}

int CGlobal::SetTurnServerUser(QString user)
{
    m_szTurnUser = user;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/TurnServerUser", user);
    return 0;
}

QString CGlobal::GetTurnServerPassword()
{
    return m_szTurnPassword;
}

int CGlobal::SetTurnServerPassword(QString password)
{
    m_szTurnPassword = password;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/TurnServerPassword", password);
    return 0;
}

QString CGlobal::GetRosterStatusText(CUserInfo::USER_INFO_STATUS status)
{
    if(CUserInfo::Online == status)
        return tr("OnLine");
    else if(CUserInfo::Away == status)
        return tr("Temporarily away");
    else if(CUserInfo::Chat == status)
        return tr("Chat");
    else if(CUserInfo::DO_NOT_DISTURB == status)
        return tr("Do not disturb");
    else if(CUserInfo::Invisible == status)
        return tr("Invisible");
    else if(CUserInfo::OffLine == status)
        return tr("OffLine");
    else
        return tr("Invisible");
}

QString CGlobal::GetRosterStatusIcon(CUserInfo::USER_INFO_STATUS status)
{
    if(CUserInfo::Online == status)
        return ":/icon/Status_available";
    else if(CUserInfo::Away == status)
        return ":/icon/Status_away";
    else if(CUserInfo::Chat == status)
        return ":/icon/Status_chat";
    else if(CUserInfo::DO_NOT_DISTURB == status)
        return ":/icon/Status_dnd";
    else if(CUserInfo::Invisible == status)
        return ":/icon/Status_invisible";
    else if(CUserInfo::OffLine == status)
        return ":/icon/Status_invisible";
    else
        return ":/icon/Status_invisible";
}

QColor CGlobal::GetRosterStatusColor(CUserInfo::USER_INFO_STATUS status)
{
    if(CUserInfo::Online == status)
        return QColor(0, 255, 0);
    else if(CUserInfo::Away == status)
        return QColor(255, 0, 255);
    else if(CUserInfo::Chat == status)
        return QColor(0, 255, 0);
    else if(CUserInfo::DO_NOT_DISTURB == status)
        return QColor(255, 0, 0);
    else if(CUserInfo::Invisible == status)
        return QColor(0, 0, 0);
    else if(CUserInfo::OffLine == status)
        return QColor(0, 0, 0);
    else
        return QColor(255, 255, 255);
}

CGlobal::E_CLOSE_TYPE CGlobal::GetCloseType()
{
    return m_CloseType;
}

int CGlobal::SetCloseType(E_CLOSE_TYPE type)
{
    m_CloseType = type;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Logout/type", type);
    return 0;
}

int CGlobal::SetMessageSendType(E_MESSAGE_SEND_TYPE type)
{
    m_MessageSendType = type;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Message/SendType", m_MessageSendType);
    return 0;
}

CGlobal::E_MESSAGE_SEND_TYPE CGlobal::GetMessageSendType()
{
    return m_MessageSendType;
}

int CGlobal::SetAutoLogin(bool bAuto)
{
    m_AutoLogin = bAuto;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/AutoLogin", bAuto);
    return 0;
}

bool CGlobal::GetAutoLogin()
{
    return m_AutoLogin;
}

int CGlobal::SetAutoLoginDelayTime(int nTime)
{
    m_nAutoLoginDelayTime = nTime;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/AutoLoginDelayTime", m_nAutoLoginDelayTime);
    return 0;
}

int CGlobal::GetAutoLoginDelayTime()
{
    return m_nAutoLoginDelayTime;
}

int CGlobal::SetNotifiationBarShowMessage(bool bShowMessage)
{
     QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
     conf.setValue("Options/NotifiationBar/ShowMessage", bShowMessage);
     m_bNotifiationBarShowMessage = bShowMessage;
     return 0;
}

bool CGlobal::IsNotifiationBarShowMessage()
{
     return m_bNotifiationBarShowMessage;
}

int CGlobal::SetNotifiationBarShowMessageDelay(int nMs)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/NotifiationBar/ShowMessageDelay", nMs);
    m_nShowMessageDelay = nMs;
    return 0;
}

int CGlobal::GetNotifiationBarShowMessageDelay()
{
    return m_nShowMessageDelay;
}

int CGlobal::SetNotifiationFlashs(bool bFlashs)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/NotifiationBar/Flashs", bFlashs);
    m_bNotifiationBarFlashs = bFlashs;
    return 0;
}

bool CGlobal::IsNotifiationFlashs()
{
    return m_bNotifiationBarFlashs;
}

int CGlobal::SetNotifiationFlashInterval(int nMs)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/NotifiationBar/FlashsInterval", nMs);
    m_nFlashInterval = nMs;
    return 0;
}

int CGlobal::GetNotifiationFlashInterval()
{
    return m_nFlashInterval;
}

int CGlobal::SetMessageSound(bool bSound)
{
    m_bMessageSound = bSound;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Message/ReceiveMessageSound", bSound);
    return 0;
}

bool CGlobal::GetMessageSound()
{
    return m_bMessageSound;
}

int CGlobal::SetAnimationHideMainWindow(int nMs)
{
    m_nAnimationHideMainWindow = nMs;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("UI/MainWindow/AnimationHide", m_nAnimationHideMainWindow);
    return 0;
}

int CGlobal::GetAnimationHideMainWindow()
{
    return m_nAnimationHideMainWindow;
}

int CGlobal::SetAnimationDuration(int nMs)
{
    m_nAnimationDuration = nMs;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("UI/MainWindow/AnimationDuration", m_nAnimationDuration);
    return 0;
}

int CGlobal::GetAnimationDuration()
{
    return m_nAnimationDuration;
}

bool CGlobal::IsAnimationHideMainWindow()
{
    return m_bAnimationHideMainWindows;
}

int CGlobal::SetIsAnimationHideMainWindow(bool bHide)
{
    m_bAnimationHideMainWindows = bHide;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("UI/MainWindow/AutoHide", m_bAnimationHideMainWindows);
    return 0;
}

int CGlobal::SetRosterShowType(E_ROSTER_SHOW_TYPE type)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Roster/ShowType", type);
    m_RosterShowType = type;
    return 0;
}

CGlobal::E_SCREEN_SHOT_TO_TYPE CGlobal::GetScreenShotToType()
{
    return m_ScreenShotToType;
}

int CGlobal::SetScreenShotToType(CGlobal::E_SCREEN_SHOT_TO_TYPE type)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/ScreenShot/ToType", type);
    m_ScreenShotToType = type;
    return 0;
}

bool CGlobal::IsHideMessageBox()
{
    return m_bHideMessageBox;
}

int CGlobal::SetHideMessageBox(bool bHide)
{
    m_bHideMessageBox = bHide;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/ScreenShot/HideMessageBox", m_bHideMessageBox);
    return 0;
}

CGlobal::E_ROSTER_SHOW_TYPE CGlobal::GetRosterShowType()
{
    return m_RosterShowType;
}

int CGlobal::GetVideoCaptureDevice()
{
    return m_nVideoCaptureDevice;
}

int CGlobal::SetVideoCaptureDevice(int nIndex)
{
    m_nVideoCaptureDevice = nIndex;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Device/Video/Capture", nIndex);
    return 0;
}

int CGlobal::GetAudioInputDevice()
{
    return m_nAudioInputDevice;
}

int CGlobal::SetAudioInputDevice(int nIndex)
{
    m_nAudioInputDevice = nIndex;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Device/Audio/Input", nIndex);
    return 0;
}

int CGlobal::GetAudioOutputDevice()
{
    return m_nAudioOutputDevice;
}

int CGlobal::SetAudioOutputDevice(int nIndex)
{
    m_nAudioOutputDevice = nIndex;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Device/Audio/Output", nIndex);
    return 0;
}

bool CGlobal::GetIsShowLocaleVideo()
{
    return m_bShowLocaleVideo;
}

int CGlobal::SetIsShowLocaleVideo(bool bShow)
{
    m_bShowLocaleVideo = bShow;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Video/Locale/Show", bShow);
    return 0;
}

bool CGlobal::GetIsMonitor()
{
    return m_bMonitor;
}

int CGlobal::SetMonitor(bool bMonitor)
{
    m_bMonitor = bMonitor;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Video/Monitor", bMonitor);
    return 0;
}

QString CGlobal::GetStyle()
{
    return m_szStyleFile;
}

QString CGlobal::GetStyleMenu()
{
    return m_szStyleMenu;
}

int CGlobal::SetStyleMenu(QString szMenu, QString szFile)
{
    m_szStyleMenu = szMenu;
    m_szStyleFile = szFile;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("UI/MenuStyleSheet", szMenu);
    conf.setValue("UI/StyleSheet", szFile);
    return 0;
}

CGlobal::E_UPDATE CGlobal::GetUpdate()
{
    return m_Update;
}

int CGlobal::SetUpdate(E_UPDATE u)
{
    m_Update = u;
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Update", m_Update);
    return 0;
}

int CGlobal::SetUpdateDate(QDateTime d)
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Update/Date", d.toString("yyyy/MM/dd hh:mm:ss"));
    return 0;
}

QDateTime CGlobal::GetUpdateDate()
{
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    QString szDate = conf.value("Options/Update/Date").toString();
    QDateTime d;
    if(szDate.isEmpty())
        return d;
    d = QDateTime::fromString(szDate, "yyyy/MM/dd hh:mm:ss");
    return d;
}
