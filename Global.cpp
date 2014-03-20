#include "Global.h"
#include "qxmpp/QXmppUtils.h"

CGlobal g_Global;

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    m_UserColor = QColor(255, 0, 0);
    m_RosterColor = QColor(0, 0, 255);
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

int CGlobal::SetUserColor(QColor color)
{
    m_UserColor = color;
    return 0;
}

QColor CGlobal::GetRosterColor()
{
    return m_RosterColor;
}

int CGlobal::SetRosterColor(QColor color)
{
    m_RosterColor = color;
    return 0;
}
