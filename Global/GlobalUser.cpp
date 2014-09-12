#include "GlobalUser.h"
#include "Global.h"
#include <QDir>
#include <QFile>
#include "qxmpp/QXmppUtils.h"

CGlobalUser::CGlobalUser(QObject *parent) :
    QObject(parent)
{
    m_bModify = false;
}

CGlobalUser::~CGlobalUser()
{
    Clean();
}

int CGlobalUser::Init(QString szLocaleJid)
{
    int nRet = 0;
    m_bModify = false;
    //从配置文件中加载数据  
    LoadFromFile(szLocaleJid);
    return nRet;
}

int CGlobalUser::Clean()
{
    int nRet = 0;
    //保存数据到配置文件中  
    if(m_bModify)
    {
        SaveToFile();
    }

    m_bModify = false;
    m_UserInforLocale.clear();
    m_UserInfoRoster.clear();
    return nRet;
}

int CGlobalUser::LoadFromFile(QString szLocaleJid)
{
    int nRet = 0;
    nRet = LoadLocaleFromFile(szLocaleJid);
    if(nRet)
        return nRet;
    nRet = LoadRosterFromFile(szLocaleJid);    
    return nRet;
}

int CGlobalUser::SaveToFile()
{
    int nRet = 0;
    nRet = SaveLocaleToFile();
    if(nRet)
        return nRet;
    nRet = SaveRosterToFile();
    return nRet;
}

int CGlobalUser::LoadLocaleFromFile(QString szLocaleJid)
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "LocaleInfo.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        LOG_MODEL_WARNING("CGlobalUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try{
        QDataStream s(&in);
        
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        //本地用户信息  
        LOG_MODEL_DEBUG("CFrmUserList", "Version:%d", nVersion);
        if(m_UserInforLocale.isNull())
         {
            QSharedPointer<CUserInfoLocale> locale(new CUserInfoLocale);
            m_UserInforLocale = locale;
        }
        s >> *m_UserInforLocale;        
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CGlobalUser", "CFrmUserList::LoadUserList exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CGlobalUser::SaveLocaleToFile()
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(CGlobal::Instance()->GetGlobalUser()->GetUserInfoLocale()->GetBareJid()) 
            + QDir::separator() + "LocaleInfo.dat";
    
    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        LOG_MODEL_WARNING("CGlobalUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }
    
    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;
        //本地用户信息  
        s << *m_UserInforLocale;     
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CGlobalUser", "CFrmUserList::SaveUserList exception");
        return -1;
    }
    
    out.close();
    return nRet;
}

int CGlobalUser::LoadRosterFromFile(QString szLocaleJid)
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "RosterInfo.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        LOG_MODEL_WARNING("CGlobalUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try{
        QDataStream s(&in);

        //版本号  
        int nVersion = 0;
        s >> nVersion;
        //本地用户信息  
        LOG_MODEL_DEBUG("CFrmUserList", "Version:%d", nVersion);
        int nSize =0;
        s >> nSize;
        while(nSize--)
        {
            QString jid;
            QSharedPointer<CUserInfoRoster> roster(new CUserInfoRoster);
            s >> jid >> *roster;
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CGlobalUser", "CFrmUserList::LoadUserList exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CGlobalUser::SaveRosterToFile()
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(CGlobal::Instance()->GetGlobalUser()->GetUserInfoLocale()->GetBareJid()) 
            + QDir::separator() + "RosterInfo.dat";

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        LOG_MODEL_WARNING("CGlobalUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;
        s << m_UserInfoRoster.size();
        QMap<QString, QSharedPointer<CUserInfoRoster> >::iterator it;
        for(it = m_UserInfoRoster.begin(); it != m_UserInfoRoster.end(); it++)
        {
            s << it.key() << *(it.value());
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CGlobalUser", "CFrmUserList::SaveUserList exception");
        return -1;
    }

    out.close();
    return nRet;
}

QSharedPointer<CUserInfoLocale> CGlobalUser::GetUserInfoLocale()
{
    return m_UserInforLocale;
}

QSharedPointer< CUserInfoRoster > CGlobalUser::GetUserInfoRoster(QString szBareJid)
{
    QSharedPointer<CUserInfoRoster> roster;
    QMap<QString, QSharedPointer<CUserInfoRoster> >::iterator it;
    it = m_UserInfoRoster.find(QXmppUtils::jidToBareJid(szBareJid));
    if(m_UserInfoRoster.end() == it)
        return roster;
    return it.value();
}

int CGlobalUser::UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid)
{
    m_bModify = true;
    if(m_UserInforLocale.isNull())
     {
        QSharedPointer<CUserInfoLocale> locale(new CUserInfoLocale);
        m_UserInforLocale = locale;
    }
    m_UserInforLocale->UpdateUserInfo(vCard, jid);
    return 0;
}

int CGlobalUser::UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem)
{
    int nRet = 0;
    m_bModify = true;
    QString jid = rosterItem.bareJid();
    QSharedPointer<CUserInfoRoster> roster = GetUserInfoRoster(jid);
    if(roster.isNull())
    {
        QSharedPointer<CUserInfoRoster> r(new CUserInfoRoster);
        roster = r;
        m_UserInfoRoster.insert(jid, roster);
    }
    nRet = roster->UpdateUserInfo(rosterItem);
    return nRet;
}

int CGlobalUser::ProcessRoster(COperateRoster* pOperateRoster, void* para)
{
    int nRet = 0;
    QMap<QString, QSharedPointer<CUserInfoRoster> >::iterator it;
    for(it = m_UserInfoRoster.begin(); it != m_UserInfoRoster.end(); it++)
    {
        nRet = pOperateRoster->ProcessRoster(it.value(), para);
        if(nRet)
            break;
    }
    return nRet;
}

int CGlobalUser::UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    m_bModify = true;
    QString szBareJid = QXmppUtils::jidToBareJid(jid);
    QSharedPointer<CUserInfoRoster> roster = GetUserInfoRoster(szBareJid);
    if(roster.isNull())
    {
        QSharedPointer<CUserInfoRoster> r(new CUserInfoRoster);
        roster = r;
        m_UserInfoRoster.insert(szBareJid, roster);
    }
    nRet = roster->UpdateUserInfo(vCard, jid);
    return nRet;
}
