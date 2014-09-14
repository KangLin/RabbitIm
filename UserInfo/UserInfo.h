#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <QDate>
#include <QImage>
#include <QDataStream>
#include <QSet>
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppVCardIq.h"

class CUserInfo : public QObject
{
    Q_OBJECT
public:
    explicit CUserInfo(QObject *parent = 0);
    virtual ~CUserInfo();

    enum USER_INFO_STATUS
    {
        Online = 0,      ///< The entity or resource is online.
        Away,            ///< The entity or resource is temporarily away.
        XA,              ///< The entity or resource is away for an extended period.
        DO_NOT_DISTURB,  ///< 别打扰  
        Chat,            ///< The entity or resource is actively interested in chatting.
        Invisible,       ///< obsolete XEP-0018: Invisible Presence
        OffLine          ///< 不在线，本程序增加的。  
    };

    virtual QString GetShowName(); //根据配置显示用户名称  
    virtual QString GetId();       //用户ID，由具体协议实现  
    virtual QString GetNick();
    virtual QDate GetBirthday();
    virtual QString GetEmail();
    virtual QString GetDescription();
    virtual QImage GetPhoto();
    virtual USER_INFO_STATUS GetStatus();//用户状态  
    virtual int SetStatus(USER_INFO_STATUS status);

    QSet<QString> &GetGroups();
    enum SUBSCRIPTION_TYPE
    {
        None = 0,   ///< the user does not have a subscription to the
        ///< contact's presence information, and the contact does
        ///< not have a subscription to the user's presence information
        From = 1,   ///< the contact has a subscription to the user's presence information,
        ///< but the user does not have a subscription to the contact's presence information
        To = 2,     ///< the user has a subscription to the contact's presence information,
        ///< but the contact does not have a subscription to the user's presence information
        Both = 3,   ///< both the user and the contact have subscriptions to each
        ///< other's presence information
        Remove = 4, ///< to delete a roster item
        NotSet = 8  ///< the subscription state was not specified
    };

    virtual SUBSCRIPTION_TYPE GetSubScriptionType();
    virtual QString GetSubscriptionTypeStr(SUBSCRIPTION_TYPE type) const;

    int GetUnReadMessageCount();
    int SetUnReadMessageCount(int nCount);
    
#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfo &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfo &roster);
#endif

protected:
    QString m_szNick;
    QDate m_Birthday;
    QString m_szEmail;
    QString m_szDescription;
    QImage m_imgPhoto;

    QSet<QString> m_Groups;
    SUBSCRIPTION_TYPE m_subscriptionType;
    int m_UnReadMessageCount;

signals:

public slots:

};

#endif // USERINFO_H
