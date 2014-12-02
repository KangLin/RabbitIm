#include "GroupChat.h"

CGroupChat::CGroupChat(QObject *parent) :
    QObject(parent), 
    m_Message(new CMessage())
{
    m_Condition = NoError;
}

QSharedPointer<CMessage> CGroupChat::GetMessage()
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
