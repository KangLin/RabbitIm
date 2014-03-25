#include "Message.h"

CMessage::CMessage()
{
}

CMessage::CMessage(QString &fromBareJid, QString &toBareJid, QString &message)
{
    m_FromBareJid = fromBareJid;
    m_ToBareJid = toBareJid;
    m_Message = message;
    m_Time = QTime::currentTime();
}

CMessage::~CMessage()
{
}

QString CMessage::GetFromBareJid()
{
    return m_FromBareJid;
}

QString CMessage::GetToBareJid()
{
    return m_ToBareJid;
}

QString CMessage::GetMessage()
{
    return m_Message;
}

QTime CMessage::GetTime()
{
    return m_Time;
}
