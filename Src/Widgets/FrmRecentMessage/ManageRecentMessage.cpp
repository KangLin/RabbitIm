#include "ManageRecentMessage.h"
#include "Global/Global.h"
#include "UserInfo/User.h"
#include <QDir>
#include <QFile>
#include "Widgets/FrmGroupChat/GroupChat.h"

CManageRecentMessage::CManageRecentMessage() : CManage()
{
}

CManageRecentMessage::~CManageRecentMessage()
{
}

int CManageRecentMessage::LoginInit(const QString &szId)
{
    bool check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(const QString&)),
                    SLOT(slotMessageUpdate(const QString&)));
    Q_ASSERT(check);
    check = connect(GETMANAGER->GetManageGroupChat().data(), SIGNAL(sigUpdateMessage(QString)),
                         SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);
    LoadFromStorage(szId);

    return 0;
}

int CManageRecentMessage::LogoutClean()
{
    SaveToStorage();
    GET_CLIENT->disconnect(this);
    GETMANAGER->GetManageGroupChat()->disconnect(this);
    m_Unread.clear();
    m_read.clear();
    return 0;
}

int CManageRecentMessage::LoadFromStorage(const QString &szId)
{
    int nRet = 0;
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "RecentMessage.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        qWarning() << "Don't open file:" << szFile;
        return -1;
    }

    try{
        QDataStream s(&in);
        
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        int nUnread = 0, nRead = 0;
        s >> nUnread;
        while(nUnread--)
        {
            QString str;
            s >> str;
            m_Unread.push_back(str);
        }
        s >> nRead;
        while(nRead--)
        {
            QString str;
            s >> str;
            m_read.push_back(str);
        }
    }
    catch(...)
    {
        qCritical() << "CManageRecentMessage::LoadFromStorage exception";
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CManageRecentMessage::SaveToStorage()
{
    int nRet = 0;
    if(USER_INFO_LOCALE.isNull())
    {
        qCritical() << "USER_INFO_LOCALE is null";
        return -1;
    }
    QString szFile = CGlobalDir::Instance()->GetDirUserData(USER_INFO_LOCALE->GetInfo()->GetId()) 
            + QDir::separator() + "RecentMessage.dat";

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        qWarning() << "Don't open file:" << szFile;
        return -1;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;
        s << m_Unread.size();
        QString str;
        foreach(str, m_Unread)
        {
            s << str;
        }
        s << m_read.size();
        foreach (str, m_read)
        {
            s << str;
        }
    }
    catch(...)
    {
        qCritical() << "CManageRecentMessage::SaveToStorage exception";
        return -1;
    }

    out.close();
    return nRet;
}

int CManageRecentMessage::GetMaxCount()
{
    return 200;
}

int CManageRecentMessage::GetUnreadCount()
{
    return m_Unread.size();
}

int CManageRecentMessage::GetAllCount()
{
    return m_Unread.size() + m_read.size();
}

QList<QString> CManageRecentMessage::GetAllMembers()
{
    QList<QString> ret;
    ret << m_Unread << m_read;
    return ret;
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
    if(m_Unread.size() + m_read.size() > GetMaxCount())
    {
        if(m_read.size() > 0)
            m_read.pop_back();
        else
            m_Unread.pop_back();
    }

    QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szId);
    if(!user.isNull())
    {
        if(user->GetMessage()->GetUnReadCount() > 0)
        {
            m_Unread.push_front(szId);
        }
        else
            m_read.push_front(szId);
    }
    else
    {
        QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
        if(!gc.isNull())
        {
            if(gc->GetMessage()->GetUnReadCount() > 0)
                m_Unread.push_front(szId);
            else
                m_read.push_front(szId);
        }
        else//不是好友消息或组消息  
            m_Unread.push_front(szId);
    }
    
    return;
}

/**
 * @brief 消息数清0时调用  
 *
 * @param szId：好友ID  
 */
void CManageRecentMessage::slotMessageClean(const QString& szId)
{
    int nIndex = m_Unread.indexOf(szId);
    if(-1 != nIndex)
        m_Unread.removeAt(nIndex);
    nIndex = m_read.indexOf(szId);
    if(-1 != nIndex)
        m_read.removeAt(nIndex);
    if(m_Unread.size() + m_read.size() > GetMaxCount())
    {
        if(m_read.size() > 0)
            m_read.pop_back();
        else
            m_Unread.pop_back();
    }
    m_read.push_front(szId);
    return;
}

int CManageRecentMessage::Remove(const QString &szId)
{
    int nIndex = m_Unread.indexOf(szId);
    if(-1 != nIndex)
        m_Unread.removeAt(nIndex);
    nIndex = m_read.indexOf(szId);
    if(-1 != nIndex)
        m_read.removeAt(nIndex);
    return 0;
}
