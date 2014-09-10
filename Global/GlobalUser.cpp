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
    return nRet;
}

int CGlobalUser::LoadFromFile(QString szLocaleJid)
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "Info.dat";

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
        //TODO:好友用户信息  
        
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CGlobalUser", "CFrmUserList::LoadUserList exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CGlobalUser::SaveToFile()
{
    int nRet = 0;
    QString szFile = CGlobal::Instance()->GetDirUserData(CGlobal::Instance()->GetGlobalUser()->GetUserInfoLocale()->GetBareJid()) 
            + QDir::separator() + "Info.dat";
    
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
        //TODO:好友信息  
        
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

int CGlobalUser::UpdateLocaleUserInfo(const QXmppVCardIq &vCard, QString jid)
{
    m_bModify = true;
    if(m_UserInforLocale.isNull())
     {
        QSharedPointer<CUserInfoLocale> locale(new CUserInfoLocale);
        m_UserInforLocale = locale;
    }
    m_UserInforLocale->UpdateLocaleUserInfo(vCard, jid);
}
