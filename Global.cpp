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

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    m_UserColor = QColor(255, 0, 0);
    m_RosterColor = QColor(0, 0, 255);

    QSettings conf(GetApplicationConfigureFile(), QSettings::IniFormat);
    m_szXmppDomain = conf.value("Login/XmppDomain", "rabbitim.com").toString();
    m_szXmppServer = conf.value("Login/XmppServer", "183.233.149.120").toString();
    m_szXmppServerPort = conf.value("Login/XmppServerPort", 5222).toInt();
    //m_szStunServer = conf.value("Login/StunServer", "stun.l.google.com").toString();
    m_szStunServer = conf.value("Login/StunServer", "183.233.149.120").toString();
    //m_szTurnServer = conf.value("Login/TurnServer", "turn.l.google.com").toString();
    m_szTurnServer = conf.value("Login/TurnServer", "183.233.149.120").toString();
    m_nStunServerPort = conf.value("Login/StunServerPort", 3478).toInt();
    m_nTurnServerPort = conf.value("Login/TurnServerPort", 3478).toInt();
    m_szTurnUser = conf.value("Login/TurnServerUser", "1").toString();
    m_szTurnPassword = conf.value("Login/TurnServerPassword", "1").toString();

    m_bNotifiationBarShowMessage = conf.value("Options/NotifiationBar/ShowMessage", true).toBool();
    m_bNotifiationBarFlashs =  conf.value("Options/NotifiationBar/Flashs", true).toBool();;

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

int CGlobal::SetJid(QString jid)
{
    m_szLocalJid = jid;
    return 0;
}

QString CGlobal::GetBareJid()
{
    return QXmppUtils::jidToBareJid(m_szLocalJid);
}

QString CGlobal::GetName()
{
    return QXmppUtils::jidToUser(m_szLocalJid);
}

QString CGlobal::GetDomain()
{
    return QXmppUtils::jidToDomain(m_szLocalJid);
}

QString CGlobal::GetResource()
{
    return QXmppUtils::jidToResource(m_szLocalJid);
}

QColor CGlobal::GetUserColor()
{
    return m_UserColor;
}

int CGlobal::SetUserColor(const QColor &color)
{
    m_UserColor = color;
    return 0;
}

QColor CGlobal::GetRosterColor() 
{
    return m_RosterColor;
}

int CGlobal::SetRosterColor(const QColor &color)
{
    m_RosterColor = color;
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

QString CGlobal::GetStatusText(QXmppPresence::AvailableStatusType status)
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

QColor CGlobal::GetStatusColor(QXmppPresence::AvailableStatusType status)
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

QString CGlobal::GetDirApplication()
{
#ifdef ANDROID
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath() + "/..";
#else
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath();
#endif
}

QString CGlobal::GetDirApplicationConfigure()
{
    return GetDirApplication() + "/conf";
}

//应用程序的配置文件 
QString CGlobal::GetApplicationConfigureFile()
{
    return GetDirApplicationConfigure() + "/app.conf";
}

QString CGlobal::GetDirUserData(const QString bareJid)
{
    QString jid = GetBareJid();
    if(!bareJid.isEmpty())
        jid = QXmppUtils::jidToBareJid(bareJid);
    jid = jid.replace("@", ".");
    QString path = GetDirApplicationConfigure() + "/Users/" + jid;
    QDir d;
    if(!d.exists(path))
    {
        if(!d.mkdir(path))
            LOG_ERROR("mkdir path fail:%s", qPrintable(path));
    }
    return path;
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
