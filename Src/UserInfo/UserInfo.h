#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <QDate>
#include <QImage>
#include <QDataStream>
#include <QSet>
#include "Global/GlobalDir.h"

/**
 * @defgroup RABBITIM_INTERFACE_USERINFO 用户信息接口类模块  
 * @ingroup RABBITIM_INTERFACE_USER
 * @brief 用户信息接口类模块，用于描述用户信息  
 */

/**
 * @class CUserInfo
 * @ingroup RABBITIM_INTERFACE_USERINFO RABBITIM_INTERFACE
 * @brief 用户信息接口类，用于描述用户信息.
 *               不同协议的用户信息由具体协议派生此类实现,
 *               使用者调用 CManager::NewUserInfo() 接口得到.
 * @see CUser
 * @see CManager
 */
class RABBITIM_SHARED_LIBRARY CUserInfo : public QObject
{
    Q_OBJECT
public:
    explicit CUserInfo(QObject *parent = 0);
    virtual ~CUserInfo();

    enum USER_INFO_STATUS
    {
        Online = 0,      ///< 在线  
        Away,            ///< 临时离开  
        OffLine,         ///< 不在线，本程序增加的。 相当于qxmpp中的XA状态  
        DO_NOT_DISTURB,  ///< 别打扰  
        Chat,            ///< 正在聊天  
        Invisible        ///< obsolete XEP-0018: 隐形状态  
    };

    virtual QString GetShowName() = 0; ///< 根据配置显示用户名称  
    virtual QString GetId() = 0;       ///< 用户ID，由具体协议实现  
    virtual int SetId(QString szId) = 0;
    virtual QString GetName() = 0;     ///< 用户的名称，由用户设置  
    virtual int SetName(const QString& szName) = 0;
    virtual QString GetNick();     ///< 呢称，由好友设置  
    virtual int SetNick(const QString& szNick);
    virtual QDate GetBirthday();
    virtual int SetBirthday(const QDate &d);
    virtual QString GetEmail();
    virtual int SetEmail(const QString& szEmail);
    virtual QString GetDescription();
    virtual int SetDescription(const QString& szDescription);
    virtual QString GetUrl() const;
    virtual int SetUrl(const QString &url);
    virtual QPixmap GetPhotoPixmap();
    virtual QImage GetPhoto();
    virtual int SetPhoto(QImage img);
    virtual int SetPhoto(QByteArray img);
    virtual USER_INFO_STATUS GetStatus();///< 用户状态  
    virtual int SetStatus(USER_INFO_STATUS status);
    virtual QString toString();

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

    virtual bool GetIsMonitor();
    virtual int SetMonitor(bool bMonitor);

#ifndef QT_NO_DATASTREAM
    virtual int LoadFromStorage(QDataStream &input);
    virtual int SaveToStorage(QDataStream &output);
#endif

protected:
    QString m_szName;
    QString m_szNick;
    QDate m_Birthday;
    QString m_szEmail;
    QString m_szDescription;
    QString m_szUrl;
    QImage m_imgPhoto;

    USER_INFO_STATUS m_Status;
    QSet<QString> m_Groups;
    SUBSCRIPTION_TYPE m_subscriptionType;

    bool m_bMonitor;
signals:

public slots:

};

#endif // USERINFO_H
