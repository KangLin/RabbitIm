#include "Global.h"

#include <QMetaType>

#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppRtpChannel.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <string>
#include "Tool.h"

CGlobal g_Global;

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    m_UserColor = QColor(255, 0, 0);
    m_RosterColor = QColor(0, 0, 255);

    m_szXmppServerHost = "192.168.10.5";
    m_szXmppServer = "rabbitim.com";
    //m_szXmppServerHost = "183.233.149.120";
    m_szXmppServerPort = 5222;
    //m_szXmppServer = "talk.renren.com";
    m_szStunServer ="stun.l.google.com";
    m_szTurnServer = m_szXmppServerHost;
    m_nStunServerPort = 3478;
    m_nTurnServerPort = 3478;
    m_szTurnUser = "1";
    m_szTurnPassword = "1";

    //如果不同线程间信号发送中的参数有自定义的数据类型，那么就必须先注册到Qt内部的类型管理器中后才能在connect()中使用
    qRegisterMetaType<QXmppVideoFrame>("QXmppVideoFrame");
    
    //窗体背景
    m_szFormBackGroundStyleSheet  = QString("background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 rgba(255, 255, 255, 100%),stop: 1 rgba(10, 144, 255, 100%));");

}

CGlobal::~CGlobal()
{
}

int CGlobal::Log(const char *pszFile, int nLine, int nLevel, const char* pszModelName, const char *pFormatString, ...)
{
    char buf[1024];
    std::string szTemp = pszFile;
    szTemp += "[";
    sprintf(buf, "%d", nLine);
    szTemp += buf;
    szTemp += "]:";
    szTemp += pszModelName;
    szTemp += ":";
    
    va_list args;
    va_start (args, pFormatString);
    vsprintf(buf, pFormatString, args);
    va_end (args);
    szTemp += buf;
    
    Q_UNUSED(nLevel);
    
    qDebug() << qPrintable(szTemp.c_str());

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

QString CGlobal::GetXmppServerHost()
{
    return m_szXmppServerHost;
}

int CGlobal::SetXmppServerHost(QString &host)
{
    m_szXmppServerHost = host;
    return 0;
}

qint16 CGlobal::GetXmppServerPort()
{
    return m_szXmppServerPort;
}

int CGlobal::SetXmppServerPort(qint16 port)
{
    m_szXmppServerPort = port;
    return 0;
}

QString CGlobal::GetXmppServer()
{
    return m_szXmppServer;
}

int CGlobal::SetXmppServer(QString server)
{
    m_szXmppServer = server;
    return 0;
}

QString CGlobal::GetTurnServer()
{
    return m_szTurnServer;
}

int CGlobal::SetTurnServer(QString &server)
{
    m_szTurnServer = server;
    return 0;
}

qint16 CGlobal::GetTurnServerPort()
{
    return m_nTurnServerPort;
}

int CGlobal::SetTurnServerPort(qint16 &port)
{
    m_nTurnServerPort = port;
    return 0;
}

QString CGlobal::GetStunServer()
{
    return m_szStunServer;
}

int CGlobal::SetStunServer(QString &server)
{
    m_szStunServer = server;
    return 0;
}

qint16 CGlobal::GetStunServerPort()
{
    return m_nStunServerPort;
}

int CGlobal::SetStunServerPort(qint16 &port)
{
    m_nStunServerPort = port;
    return 0;
}

QString CGlobal::GetTurnServerUser()
{
    return m_szTurnUser;
}

int CGlobal::SetTurnServerUser(QString &user)
{
    m_szTurnUser = user;
    return 0;
}

QString CGlobal::GetTurnServerPassword()
{
    return m_szTurnPassword;
}

int CGlobal::SetTurnServerPassword(QString &password)
{
    m_szTurnPassword = password;
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
    return qApp->applicationDirPath();
}

QString CGlobal::GetDirApplicationConfigure()
{
    return GetDirApplication() + "/conf";
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

//得到默认样式
QString CGlobal::GetStyleSheet(const __STYLE_SHEET_TYPE t)
{
    switch(t)
    {
    case STYPLE_SHEET_FORM_BACKGROUND:
        return m_szFormBackGroundStyleSheet;
    case STYPLE_SHEET_DEFAULT:
    default:
        LOG_MODEL_ERROR("global", "don't style sheet:%d", t);
        break;
    };
    return "";
}

//设置窗口样式,返回原来窗口样式
QString CGlobal::SetStyleSheet(const QString szStyleSheet, const __STYLE_SHEET_TYPE t)
{
    QString szOld = GetStyleSheet(t);
    switch (t) {
    case STYPLE_SHEET_FORM_BACKGROUND:
        m_szFormBackGroundStyleSheet = szStyleSheet;
        break;
    default:
        LOG_MODEL_ERROR("global", "don't style sheet:%d", t);
        break;
    }
    return szOld;
}

 //设置窗口样式,返回原来窗口样式
QString CGlobal::SetStyleSheet(QWidget *pWidget, const __STYLE_SHEET_TYPE t)
{
    switch(t)
    {
    case STYPLE_SHEET_FORM_BACKGROUND:
    {        
        QString szStyle = GetStyleSheet(t);
        QString szRet = pWidget->styleSheet();
        pWidget->setStyleSheet(szStyle);
        //CTool::SetAllChildrenTransparent(pWidget);
        return szRet;
    }
    };
}
