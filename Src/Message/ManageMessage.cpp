#include "ManageMessage.h"
#include "ChatActions/MessageAction.h"
#include "Global/Global.h"

CManageMessage::CManageMessage(QObject *parent) :
    QObject(parent)
{
    m_nUnreadCount = 0;
}

CManageMessage::~CManageMessage()
{
    m_Message.clear();
    LOG_MODEL_DEBUG("CMessage", "CMessage::~CMessage");
}

int CManageMessage::GetUnReadCount()
{
    return m_nUnreadCount;
}

QSharedPointer<CChatAction> CManageMessage::AddMessage(const QString& szId, 
                                                 const QString &szMessage,
                                                 const bool bMe, const QTime time)
{
    QSharedPointer<CChatAction> chat(new CMessageAction(szId, szMessage, time, bMe));
    m_Message.push_back(chat);
    m_nUnreadCount++;
    return chat;
}

int CManageMessage::AddMessage(QSharedPointer<CChatAction> chatAction)
{
    m_Message.push_back(chatAction);
    m_nUnreadCount++;
    return 0;
}

std::vector<QSharedPointer<CChatAction> > CManageMessage::GetUnreadMessage()
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
