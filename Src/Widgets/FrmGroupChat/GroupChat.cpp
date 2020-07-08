#include "GroupChat.h"

CGroupChat::CGroupChat(QObject *parent) :
    QObject(parent), 
    m_Message(new CManageMessage())
{
    m_Condition = NoError;
}

QString CGroupChat::Password()
{
    return m_szPassword;
}

int CGroupChat::SetPassword(const QString &szPassword)
{
    m_szPassword = szPassword;
    return 0;
}

QSharedPointer<CManageMessage> CGroupChat::GetMessage()
{
    return m_Message;
}

QIcon CGroupChat::Icon()
{
    return QIcon(":/icon/Conference");
}

int CGroupChat::SetError(Condition c)
{
    m_Condition = c;
    return 0;
}

CGroupChat::Condition CGroupChat::Error()
{
    return m_Condition;
}
