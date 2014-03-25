#include "Global.h"
#include "qxmpp/QXmppUtils.h"

CGlobal g_Global;

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    m_UserColor = QColor(255, 0, 0);
    m_RosterColor = QColor(0, 0, 255);

    m_szXmppServer = "183.62.225.76";
    m_szStunServer = "183.62.225.76";
    m_szTurnServer = "183.62.225.76";
    m_nStunServerPort = 3478;
    m_nTurnServerPort = 13478;
}

CGlobal::~CGlobal()
{
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

int CGlobal::SetUserColor(QColor &color)
{
    m_UserColor = color;
    return 0;
}

QColor CGlobal::GetRosterColor()
{
    return m_RosterColor;
}

int CGlobal::SetRosterColor(QColor &color)
{
    m_RosterColor = color;
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

int CGlobal::GetTurnServerPort()
{
    return m_nTurnServerPort;
}

int CGlobal::SetTurnServerPort(int &port)
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

int CGlobal::GetStunServerPort()
{
    return m_nStunServerPort;
}

int CGlobal::SetStunServerPort(int &port)
{
    m_nStunServerPort = port;
    return 0;
}
