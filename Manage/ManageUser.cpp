#include "ManageUser.h"
#include <QDir>
#include <QFile>
#include "Global/Global.h"

CManageUser::CManageUser(QObject *parent) :
    CManage(parent)
{
    m_bModify = false;
}

CManageUser::~CManageUser()
{
}

int CManageUser::LoginInit(const QString &szId)
{
    int nRet = 0;
    m_bModify = false;
    //从配置文件中加载数据  
    LoadFromFile(szId);
    return nRet;
}

int CManageUser::LogoutClean()
{
    int nRet = 0;
    //保存数据到配置文件中  
    if(m_bModify)
    {
        SaveToStorage();
    }

    m_bModify = false;
    m_UserLocale.clear();
    m_UseRoster.clear();
    return nRet;
}

int CManageUser::SetModify(bool bModify)
{
    m_bModify = bModify;
    return 0;
}

int CManageUser::LoadFromFile(QString szId)
{
    int nRet = 0;
    nRet = LoadLocaleFromStorage(szId);
    if(nRet)
        return nRet;
    nRet = LoadRosterFromStorage(szId);    
    return nRet;
}

int CManageUser::SaveToStorage()
{
    int nRet = 0;
    nRet = SaveLocaleToStorage();
    if(nRet)
    {
        LOG_MODEL_ERROR("CManageUser", "SaveLocaleToStorage fail:%d", nRet);
        return nRet;
    }
    nRet = SaveRosterToStorage();
    return nRet;
}

int CManageUser::LoadLocaleFromStorage(const QString &szId)
{
    int nRet = 0;
    QString szFile = GetLocaleFile(szId);

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        LOG_MODEL_WARNING("CManageUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try{
        QDataStream s(&in);
        
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        //本地用户信息  
        LOG_MODEL_DEBUG("CManageUser", "Version:%d", nVersion);
        if(m_UserLocale.isNull())
         {
            m_UserLocale = NewUser();
        }
        //s >> *m_UserInforLocale;        
        m_UserLocale->GetInfo()->LoadFromStorage(s);
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CManageUser", "CManageUser::LoadLocaleFromStorage exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CManageUser::SaveLocaleToStorage()
{
    int nRet = 0;
    if(GetUserInfoLocale().isNull())
        return -1;
    QString szFile = GetLocaleFile(GetUserInfoLocale()->GetInfo()->GetId());

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        LOG_MODEL_WARNING("CManageUser", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;
        //本地用户信息  
        //s << *m_UserInforLocale;     
        m_UserLocale->GetInfo()->SaveToStorage(s);
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CManageUser", "CManageUser::SaveLocaleToStorage exception");
        return -1;
    }

    out.close();
    return nRet;
}

int CManageUser::LoadRosterFromStorage(QString szId)
{
    int nRet = 0;
    QString szFile = GetRosterFile(szId);
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
            QSharedPointer<CUser> roster = NewUser();
            roster->GetInfo()->LoadFromStorage(s);
            m_UseRoster.insert(roster->GetInfo()->GetId(), roster);
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

int CManageUser::SaveRosterToStorage()
{
    int nRet = 0;
    QString szFile = GetRosterFile(GetUserInfoLocale()->GetInfo()->GetId());

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
        s << m_UseRoster.size();
        QMap<QString, QSharedPointer<CUser> >::iterator it;
        for(it = m_UseRoster.begin(); it != m_UseRoster.end(); it++)
        {
            it.value()->GetInfo()->SaveToStorage(s);
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

QSharedPointer<CUser> CManageUser::GetUserInfoLocale()
{
    return m_UserLocale;
}

QSharedPointer<CUser> CManageUser::GetUserInfoRoster(const QString &szId)
{
    QMap<QString, QSharedPointer<CUser> >::iterator it;
    it = m_UseRoster.find(szId);
    if(m_UseRoster.end() == it)
    {
        LOG_MODEL_WARNING("GlobalUser", "Don't find roster:%s", szId.toStdString().c_str());
        QSharedPointer<CUser> roster;
        return roster;
    }

    return it.value();
}

QSharedPointer<CUser> CManageUser::AddUserInfoRoster(const QString &szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_ERROR("CGlobalUser", "The CGlobalUser::AddUserInfoRoster function must be implemented by derived classes");
    QSharedPointer<CUser> user;
    return user;
}

int CManageUser::ProcessRoster(COperateRoster* pOperateRoster, void* para)
{
    int nRet = 0;
    QMap<QString, QSharedPointer<CUser> >::iterator it;
    for(it = m_UseRoster.begin(); it != m_UseRoster.end(); it++)
    {
        nRet = pOperateRoster->ProcessRoster(it.value()->GetInfo(), para);
        if(nRet)
            break;
    }
    return nRet;
}

int CManageUser::RemoveUserInfoRoster(const QString &szId)
{
    return !m_UseRoster.remove(szId);
}

QString CManageUser::GetLocaleFile(const QString &szId)
{
    return CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "LocaleInfo.dat";
}

QString CManageUser::GetRosterFile(const QString &szId)
{
    return CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "RosterInfo.dat";
}

QSharedPointer<CUser> CManageUser::NewUser()
{
    QSharedPointer<CUser> user(new CUser);
    return user;
}

