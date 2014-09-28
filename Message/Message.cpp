#include "Message.h"
#include "ChatActions/MessageAction.h"

CMessage::CMessage(QObject *parent) :
    QObject(parent)
{
}

QSharedPointer<CChatAction> CMessage::AddMessage(const QString& szId, const QString &szMessage, const QDate date, const bool bMe) const
{
    QSharedPointer<CChatAction> chat(new CMessageAction(szId, szMessage, date.toString(), bMe));
    m_Message.push_back(chat);
    return chat;
}
