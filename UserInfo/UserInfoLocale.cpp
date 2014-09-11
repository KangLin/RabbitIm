#include "UserInfoLocale.h"
#include "Global/Global.h"
#include <QSettings>
#include <QBuffer>
#include <QImageReader>
#include <QImageWriter>

CUserInfoLocale::CUserInfoLocale(QObject *parent) : CUserInfo(parent)
{
}

QXmppPresence::AvailableStatusType CUserInfoLocale::GetStatus()
{
    return m_LocalStatus;
}

int CUserInfoLocale::SetStatus(QXmppPresence::AvailableStatusType status)
{
    m_LocalStatus = status;
    return 0;

    QSettings conf(CGlobal::Instance()->GetUserConfigureFile(this->GetBareJid()), QSettings::IniFormat);
    conf.setValue("Login/LoginState", status);
    return 0;
}

int CUserInfoLocale::UpdateUserInfo(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    nRet = CUserInfo::UpdateUserInfo(vCard, jid);
    if(nRet)
        return nRet;

    //保存头像到本地  
    QImageWriter imageWriter(CGlobal::Instance()->GetFileUserAvatar(GetBareJid()), "png");
    if(!imageWriter.write(GetPhoto()))
        LOG_MODEL_ERROR("CUserInfo", "Save avater error, %s", imageWriter.errorString().toStdString().c_str());
    return 0;
}

QDataStream & operator <<(QDataStream &output, const CUserInfoLocale &roster)
{
    /*output << roster.m_szJid 
                   << roster.m_szNick 
                   << roster.m_szName 
                   << roster.m_szEmail 
                   << roster.m_szDescription
                   << roster.m_Birthday;//*/
    output << (CUserInfo&)roster;
    return output;
}

QDataStream & operator >>(QDataStream &input, CUserInfoLocale &roster)
{
    /*input >> roster.m_szJid
               >> roster.m_szNick
               >> roster.m_szName
               >> roster.m_szEmail
               >> roster.m_szDescription
               >> roster.m_Birthday;//*/
    input >> (CUserInfo&)roster;
    return input;
}

