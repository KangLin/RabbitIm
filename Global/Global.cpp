#include "Global.h"

#include <QMetaType>

#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppRtpChannel.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <string>
#include <QSettings>
#include "Tool.h"
#include "Widgets/FrmUserList/Roster.h"
#include "MainWindow.h"

CGlobal::CGlobal(QObject *parent) :
    QObject(parent),
    m_XmppClient(parent)
{
    m_pMainWindow = NULL;
    QSettings conf(GetApplicationConfigureFile(), QSettings::IniFormat);
    m_LocalStatus = (QXmppPresence::AvailableStatusType)conf.value("Login/LoginState", QXmppPresence::Online).toInt();
    m_UserColor = GetColorFormConf("Options/User/LocalColor", QColor(255, 0, 0));
    m_RosterColor = GetColorFormConf("Options/User/RosterColor", QColor(0, 0, 255));
    m_UserMessageColor = GetColorFormConf("Options/User/LocalMessageColor", QColor(0, 0, 0));
    m_RosterMessageColor = GetColorFormConf("Options/User/RosterMessageColor", QColor(0, 0, 0));
    m_UnreadMessageCountColor = GetColorFormConf("Options/User/UnreadMessageCountColor", QColor(255, 0, 0));

#ifdef MOBILE
    m_MessageSendType = (E_MESSAGE_SEND_TYPE)conf.value("Options/Message/SendType", E_MESSAGE_SEND_TYPE_ENTER).toInt();
#else
    m_MessageSendType = (E_MESSAGE_SEND_TYPE)conf.value("Options/Message/SendType", E_MESSAGE_SEND_TYPE_CTRL_ENTER).toInt();
#endif

    m_szXmppDomain = conf.value("Login/XmppDomain", "rabbitim.com").toString();
    //m_szXmppDomain = conf.value("Login/XmppDomain", "p1234.vicp.net").toString();
    //m_szXmppServer = conf.value("Login/XmppServer", "183.233.149.120").toString();
    m_szXmppServer = conf.value("Login/XmppServer", "182.254.185.29").toString();
    m_szXmppServerPort = conf.value("Login/XmppServerPort", 5222).toInt();
    //m_szStunServer = conf.value("Login/StunServer", "stun.l.google.com").toString();
    m_szStunServer = conf.value("Login/StunServer", "183.233.149.120").toString();
    //m_szTurnServer = conf.value("Login/TurnServer", "turn.l.google.com").toString();
    m_szTurnServer = conf.value("Login/TurnServer", "183.233.149.120").toString();
    m_nStunServerPort = conf.value("Login/StunServerPort", 3478).toInt();
    m_nTurnServerPort = conf.value("Login/TurnServerPort", 3478).toInt();
    m_szTurnUser = conf.value("Login/TurnServerUser", "1").toString();
    m_szTurnPassword = conf.value("Login/TurnServerPassword", "1").toString();

    m_AutoLogin = conf.value("Login/AutoLogin", false).toBool();
    m_nAutoLoginDelayTime = conf.value("Login/AutoLoginDelayTime", "3").toInt();
    m_bNotifiationBarShowMessage = conf.value("Options/NotifiationBar/ShowMessage", true).toBool();
    m_nShowMessageDelay = conf.value("Options/NotifiationBar/ShowMessageDelay", 10000).toInt();
    m_bNotifiationBarFlashs =  conf.value("Options/NotifiationBar/Flashs", true).toBool();
    m_nFlashInterval = conf.value("Options/NotifiationBar/FlashsInterval", 500).toInt();
    m_bMessageSound = conf.value("Options/Message/ReceiveMessageSound", true).toBool();

    m_RosterShowType = (E_ROSTER_SHOW_TYPE)conf.value("Options/Roster/ShowType", E_ROSTER_SHOW_NAME).toInt();
    m_ScreenShotToType = (E_SCREEN_SHOT_TO_TYPE)conf.value("Options/ScreenShot/ToType", E_TO_SEND).toInt();

    //如果不同线程间信号发送中的参数有自定义的数据类型，那么就必须先注册到Qt内部的类型管理器中后才能在connect()中使用 
    qRegisterMetaType<QXmppVideoFrame>("QXmppVideoFrame");
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

int CGlobal::Log(const char *pszFile, int nLine, int nLevel, const char* pszModelName, const char *pFormatString, ...)
{
    char buf[1024];
    std::string szTemp = pszFile;
    szTemp += "(";
    sprintf(buf, "%d", nLine);
    szTemp += buf;
    szTemp += "):";
    szTemp += pszModelName;
    szTemp += ":";

    va_list args;
    va_start (args, pFormatString);
    int nRet = vsnprintf(buf, 1024, pFormatString, args);
    va_end (args);
    if(nRet < 0 || nRet >= 1024)
    {
        LOG_MODEL_ERROR("Global", "vsprintf is fail:%d", nRet);
        return nRet;
    }
    szTemp += buf;

    Q_UNUSED(nLevel);

    qDebug() << szTemp.c_str();

    return 0;
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

CXmppClient* CGlobal::GetXmppClient()
{
    return &m_XmppClient;
}

QSharedPointer<CGlobalUser> CGlobal::GetGlobalUser()
{
    if(m_GlobalUser.isNull())
    {
        QSharedPointer<CGlobalUser> d(new CGlobalUser);
        m_GlobalUser = d;
    }
    return m_GlobalUser;
}

QXmppPresence::AvailableStatusType CGlobal::GetStatus()
{
    return m_LocalStatus;
}

int CGlobal::SetStatus(QXmppPresence::AvailableStatusType status)
{
    m_LocalStatus = status;
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/LoginState", status);
    return 0;
}

QColor CGlobal::GetColorFormConf(const QString &Key, const QColor &def)
{
    QSettings conf(GetApplicationConfigureFile(), QSettings::IniFormat);
    return QColor(conf.value(Key + "R", def.red()).toInt(),
           conf.value(Key + "G", def.green()).toInt(),
           conf.value(Key + "B", def.blue()).toInt());
}

int CGlobal::SetColorToConf(const QString &Key, const QColor &color)
{
    QSettings conf(GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/TurnServerPassword", password);
    return 0;
}

QString CGlobal::GetRosterStatusText(QXmppPresence::AvailableStatusType status)
{
    if(QXmppPresence::Online == status)
        return tr("OnLine");
    else if(QXmppPresence::Away == status)
        return tr("Temporarily away");
    else if(QXmppPresence::Chat == status)
        return tr("Chat");
    else if(QXmppPresence::DND == status)
        return tr("Do not disturb");
    else if(QXmppPresence::Invisible == status)
        return tr("Invisible");
    else if(QXmppPresence::XA == status)
        return tr("Away for an extended period");
    else
        return tr("Invisible");
}

QString CGlobal::GetRosterStatusIcon(QXmppPresence::AvailableStatusType status)
{
    if(QXmppPresence::Online == status)
        return ":/icon/Status_available";
    else if(QXmppPresence::Away == status)
        return ":/icon/Status_away";
    else if(QXmppPresence::Chat == status)
        return ":/icon/Status_chat";
    else if(QXmppPresence::DND == status)
        return ":/icon/Status_dnd";
    else if(QXmppPresence::Invisible == status)
        return ":/icon/Status_invisible";
    else if(QXmppPresence::XA == status)
        return ":/icon/Status_invisible";
    else
        return ":/icon/Status_invisible";
}

QColor CGlobal::GetRosterStatusColor(QXmppPresence::AvailableStatusType status)
{
    if(QXmppPresence::Online == status)
        return QColor(0, 255, 0);
    else if(QXmppPresence::Away == status)
        return QColor(255, 0, 255);
    else if(QXmppPresence::Chat == status)
        return QColor(0, 255, 0);
    else if(QXmppPresence::DND == status)
        return QColor(255, 0, 0);
    else if(QXmppPresence::Invisible == status)
        return QColor(255, 255, 255);
    else if(QXmppPresence::XA == status)
        return QColor(255, 0, 255);
    else
        return QColor(255, 255, 255);
}

int CGlobal::SetMessageSendType(E_MESSAGE_SEND_TYPE type)
{
    m_MessageSendType = type;
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Message/SendType", m_MessageSendType);
    return 0;
}

CGlobal::E_MESSAGE_SEND_TYPE CGlobal::GetMessageSendType()
{
    return m_MessageSendType;
}

QString CGlobal::GetDirApplication()
{
#ifdef ANDROID
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath() + QDir::separator() + "..";
#else
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath();
#endif
}

QString CGlobal::GetDirApplicationConfigure()
{
    return GetDirApplication() + QDir::separator() + "conf";
}

//应用程序的配置文件  
QString CGlobal::GetApplicationConfigureFile()
{
    return GetDirApplicationConfigure() + QDir::separator() + "app.conf";
}

QString CGlobal::GetDirTranslate()
{
    return GetDirApplication() + QDir::separator() + "translations";
}

QString CGlobal::GetDirUserData(const QString bareJid)
{
    QString jid;
    if(bareJid.isEmpty())
    {
        if(!GetGlobalUser().isNull() && !GetGlobalUser()->GetUserInfoLocale().isNull())
            jid = GetGlobalUser()->GetUserInfoLocale()->GetBareJid();
        else
        {
            LOG_MODEL_ERROR("Global", "Don't initialization GetGlobalUser or GetUserInfoLocale");
            Q_ASSERT(false);
        }
    }
    else
        jid = QXmppUtils::jidToBareJid(bareJid);
    jid = jid.replace("@", ".");
    QString path = GetDirApplicationConfigure() + QDir::separator() + "Users";
    QDir d;
    if(!d.exists(path))
    {
        if(!d.mkdir(path))
            LOG_ERROR("mkdir path fail:%s", qPrintable(path));
    }
    path = path + QDir::separator() + jid;
    if(!d.exists(path))
    {
        if(!d.mkdir(path))
            LOG_ERROR("mkdir path fail:%s", qPrintable(path));
    }
    return path;
}

QString CGlobal::GetDirUserAvatar()
{
    QString dirHeads = GetDirUserData() + QDir::separator() + "Heads";
    QDir d;
    if(!d.exists(dirHeads))
        if(!d.mkdir(dirHeads))
            LOG_MODEL_ERROR("CGlobal", "mkdir GetUserDataAvatar error:%s", qPrintable(dirHeads));
    return dirHeads;
}

QString CGlobal::GetFileUserAvatar(QString bareJid)
{
    QString jid;
    if(!bareJid.isEmpty())
        jid = QXmppUtils::jidToBareJid(bareJid);
    jid = jid.replace("@", ".");

    return GetDirUserAvatar() + QDir::separator() + jid + ".png";
}

QString CGlobal::GetUserConfigureFile(QString jid)
{
    return GetDirUserData(jid) + QDir::separator() + "user.conf";
}

int CGlobal::SetAutoLogin(bool bAuto)
{
    m_AutoLogin = bAuto;
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Login/AutoLoginDelayTime", m_nAutoLoginDelayTime);
    return 0;
}

int CGlobal::GetAutoLoginDelayTime()
{
    return m_nAutoLoginDelayTime;
}

int CGlobal::SetNotifiationBarShowMessage(bool bShowMessage)
{
     QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/Message/ReceiveMessageSound", bSound);
    return 0;
}

bool CGlobal::GetMessageSound()
{
    return m_bMessageSound;
}

int CGlobal::SetRosterShowType(E_ROSTER_SHOW_TYPE type)
{
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    conf.setValue("Options/ScreenShot/ToType", type);
    m_ScreenShotToType = type;
    return 0;
}

CGlobal::E_ROSTER_SHOW_TYPE CGlobal::GetRosterShowType()
{
    return m_RosterShowType;
}
