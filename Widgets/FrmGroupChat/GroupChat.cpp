#include "GroupChat.h"

CGroupChat::CGroupChat(QObject *parent) :
    QObject(parent), 
    m_Message(new CMessage())
{
}

QSharedPointer<CMessage> CGroupChat::GetMessage()
{
    return m_Message;
}

QIcon CGroupChat::Icon()
{
    return QIcon(":/icon/Conference");
}
