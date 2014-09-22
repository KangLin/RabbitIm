#include "GlobalUser.h"
#include "Global.h"
#include <QDir>
#include <QFile>

CGlobalUser::CGlobalUser(QObject *parent) :
    QObject(parent)
{
    m_bModify = false;
}

CGlobalUser::~CGlobalUser()
{
    Clean();
}

int CGlobalUser::Init(QString szId)
{
    int nRet = 0;
    m_bModify = false;
    //从配置文件中加载数据  
    LoadFromFile(szId);
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

int CGlobalUser::SetModify(bool bModify)
{
    m_bModify = bModify;
    return 0;
}

int CGlobalUser::LoadFromFile(QString szId)
{
    int nRet = 0;
    nRet = LoadLocaleFromFile(szId);
    if(nRet)
        return nRet;
    nRet = LoadRosterFromFile(szId);    
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

int CGlobalUser::LoadLocaleFromFile(const QString &szId)
{
    int nRet = 0;
    QString szFile = GetLocaleFile(szId);

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
            m_UserInforLocale = NewUserInfo();
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
    QString szFile = GetLocaleFile(GetUserInfoLocale()->GetId());

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

int CGlobalUser::LoadRosterFromFile(QString szId)
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
            QString jid;
            QSharedPointer<CUserInfo> roster = NewUserInfo();
            s >> jid >> *roster;
            m_UserInfoRoster.insert(jid, roster);
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
    QString szFile = GetRosterFile(GetUserInfoLocale()->GetId());

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
        QMap<QString, QSharedPointer<CUserInfo> >::iterator it;
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

QSharedPointer<CUserInfo> CGlobalUser::GetUserInfoLocale()
{
    return m_UserInforLocale;
}

QSharedPointer<CUserInfo> CGlobalUser::GetUserInfoRoster(const QString &szId)
{
    QMap<QString, QSharedPointer<CUserInfo> >::iterator it;
    it = m_UserInfoRoster.find(szId);
    if(m_UserInfoRoster.end() == it)
    {
        LOG_MODEL_WARNING("GlobalUser", "Don't find roster:%s", szId.toStdString().c_str());
        QSharedPointer<CUserInfo> roster;
        return roster;
    }

    return it.value();
}

QSharedPointer<CUserInfo> CGlobalUser::AddUserInfoRoster(const QString &szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_ERROR("CGlobalUser", "The CGlobalUser::AddUserInfoRoster function must be implemented by derived classes");
    QSharedPointer<CUserInfo> user;
    return user;
}

int CGlobalUser::ProcessRoster(COperateRoster* pOperateRoster, void* para)
{
    int nRet = 0;
    QMap<QString, QSharedPointer<CUserInfo> >::iterator it;
    for(it = m_UserInfoRoster.begin(); it != m_UserInfoRoster.end(); it++)
    {
        nRet = pOperateRoster->ProcessRoster(it.value(), para);
        if(nRet)
            break;
    }
    return nRet;
}

int CGlobalUser::RemoveUserInfoRoster(const QString &szId)
{
    return !m_UserInfoRoster.remove(szId);
}

QString CGlobalUser::GetLocaleFile(const QString &szId)
{
    return CGlobal::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "LocaleInfo.dat";
}

QString CGlobalUser::GetRosterFile(const QString &szId)
{
    return CGlobal::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "RosterInfo.dat";
}

QSharedPointer<CUserInfo> CGlobalUser::NewUserInfo()
{
    QSharedPointer<CUserInfo> user(new CUserInfo);
    return user;
}

