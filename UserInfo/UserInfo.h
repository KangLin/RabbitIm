#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <QDate>
#include <QImage>
#include "qxmpp/QXmppPresence.h"

class CUserInfo : public QObject
{
    Q_OBJECT
public:
    explicit CUserInfo(QObject *parent = 0);

    virtual QString GetShowName();//根据配置显示用户名称  
    virtual QString GetName();
    virtual QString GetJid();
    virtual QString GetBareJid();
    virtual QString GetDomain();
    virtual QString GetResource();
    virtual QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    virtual int SetStatus(QXmppPresence::AvailableStatusType status);
    virtual QString GetNick();
    virtual QDate GetBirthday();
    virtual QString GetEmail();
    virtual QString GetDescription();
    virtual QImage GetPhoto();

protected:
    QString m_szJid;
    QString m_szFullName;
    QString m_szNick;
    QDate m_Birthday;
    QString m_szEmail;
    QString m_szDescription;
    QImage m_imgPhoto;
    QXmppPresence::AvailableStatusType m_LocalStatus;//本地用户的状态  
    
signals:
    
public slots:
    
};

#endif // USERINFO_H
