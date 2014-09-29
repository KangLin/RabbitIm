#include "Message.h"
#include "ChatActions/MessageAction.h"
#include "Global/Global.h"

CMessage::CMessage(QObject *parent) :
    QObject(parent)
{
    m_nUnreadCount = 0;
}

int CMessage::GetUnReadCount()
{
    return m_nUnreadCount;
}

QSharedPointer<CChatAction> CMessage::AddMessage(const QString& szId, 
                                                 const QString &szMessage,
                                                 const bool bMe, const QDate date)
{
    QSharedPointer<CChatAction> chat(new CMessageAction(szId, szMessage, date.toString(), bMe));
    m_Message.push_back(chat);
    m_nUnreadCount++;
    return chat;
}

std::vector<QSharedPointer<CChatAction> > CMessage::GetUnreadMessage()
{
#ifdef DEBUG
    if(!m_nUnreadCount)
    {
        LOG_MODEL_DEBUG("CMessage", "CMessage::GetUnreadMessage() unread count is zero");
    }
#endif

    std::vector<QSharedPointer<CChatAction> > ret(m_Message.end() - m_nUnreadCount, m_Message.end());
    m_nUnreadCount = 0;
    return ret;
}
