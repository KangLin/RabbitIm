#include "ManageGroupChat.h"
#include "Global/Global.h"
#include <QDir>
#include <QFile>

CManageGroupChat::CManageGroupChat(QObject *parent) :
    CManage(parent)
{
}

CManageGroupChat::~CManageGroupChat()
{
}

int CManageGroupChat::LoginInit(const QString &szId)
{
    int nRet = 0;
    nRet = LoadFromStorage(szId);
    return nRet;
}

int CManageGroupChat::LogoutClean()
{
    int nRet = 0;
    nRet = SaveToStorage();
    return nRet;
}

int CManageGroupChat::LoadFromStorage(const QString &szId)
{
    int nRet = 0;
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "GroupChat.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        qWarning("Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try{
        QDataStream s(&in);

        //版本号  
        int nVersion = 0;
        s >> nVersion;
        int nSize = 0;
        s >> nSize;
        while(nSize--)
        {
            QString szId, szPassword, szNick;
            s >> szId >> szPassword >> szNick;
            Join(szId, szPassword, szNick);
        }
    }
    catch(...)
    {
        qCritical("CManageGroupChat::LoadFromStorage exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CManageGroupChat::SaveToStorage()
{
    int nRet = 0;
    if(USER_INFO_LOCALE.isNull())
    {
        qCritical("USER_INFO_LOCALE is null");
        return -1;
    }
    QString szFile = CGlobalDir::Instance()->GetDirUserData(USER_INFO_LOCALE->GetInfo()->GetId()) 
            + QDir::separator() + "GroupChat.dat";

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        qWarning("Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion 
          << m_GroupChat.size();
        QMap<QString, QSharedPointer<CGroupChat> >::iterator it;
        for(it = m_GroupChat.begin(); it != m_GroupChat.end(); it++)
        {
            s << it.key() << it.value()->Password() << it.value()->Nick();
        }
    }
    catch(...)
    {
        qCritical("CManageGroupChat::SaveToStorage exception");
        return -1;
    }

    out.close();
    return nRet;
}

int CManageGroupChat::slotLeave(const QString &szId)
{
    QMap<QString, QSharedPointer<CGroupChat> >::iterator it;
    it = m_GroupChat.find(szId);
    if(m_GroupChat.end() == it)
    {
        qDebug("Leave:Don't find %s", szId.toStdString().c_str());
        return -1;
    }
    emit sigLeave(szId);
    m_GroupChat.erase(it);
    return -2;
}

int CManageGroupChat::slotError(const QString &szId, CGroupChat::Condition c, int errorcode)
{
    emit sigError(szId, c, errorcode);
    return 0;
}

void CManageGroupChat::slotInvitationReceived(const QString &Id, const QString &inviter, const QString &reason)
{
    Q_UNUSED(Id);
    Q_UNUSED(inviter);
    Q_UNUSED(reason);
    return;
}
