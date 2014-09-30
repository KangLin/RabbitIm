#include "ManageRecentMessage.h"
#include "Global/Global.h"
#include "UserInfo/User.h"

CManageRecentMessage::CManageRecentMessage() : QObject()
{
}

CManageRecentMessage::~CManageRecentMessage()
{
}

int CManageRecentMessage::Init(const QString &szId)
{
    Q_UNUSED(szId);
    bool check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(const QString&)),
                    SLOT(slotMessageUpdate(const QString&)));
    Q_ASSERT(check);
    return 0;
}

int CManageRecentMessage::Clean()
{
    GET_CLIENT->disconnect(this);
    m_Unread.clear();
    m_read.clear();
    return 0;
}

int CManageRecentMessage::GetUnreadCount()
{
    return m_Unread.size();
}

int CManageRecentMessage::ShowLastMessageDialog()
{
    if(m_Unread.isEmpty())
        return -1;
    QString szId = m_Unread.at(0);
    m_Unread.pop_front();
    CGlobal::Instance()->GetManager()->GetManageMessageDialog()->ShowDialog(szId);
    m_read.push_back(szId);
    return 0;
}

void CManageRecentMessage::slotMessageUpdate(const QString& szId)
{
    int nIndex = m_Unread.indexOf(szId);
    if(-1 != nIndex)
        m_Unread.removeAt(nIndex);
    nIndex = m_read.indexOf(szId);
    if(-1 != nIndex)
        m_read.removeAt(nIndex);
    QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szId);
    if(user.isNull())
    {
        //不是好友  
        m_Unread.push_front(szId);
    }
    else if(user->GetMessage()->GetUnReadCount() <= 0)
    {
        m_read.push_front(szId);
    }
    else
        m_Unread.push_front(szId);
    return;
}

void CManageRecentMessage::slotMessageClean(const QString& szId)
{
    int nIndex = m_Unread.indexOf(szId);
    if(-1 != nIndex)
        m_Unread.removeAt(nIndex);
    nIndex = m_read.indexOf(szId);
    if(-1 != nIndex)
        m_read.removeAt(nIndex);
    m_read.push_front(szId);
    return;
}
