#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QObject>
#include "Message/Message.h"
#include <QIcon>

class CGroupChat : public QObject
{
    Q_OBJECT
public:
    explicit CGroupChat(QObject *parent = 0);
    virtual QString Id() = 0;//房间ID  
    virtual QString ShowName() = 0;//显示的房间  
    virtual QString Nick() = 0;//呢称  
    virtual QString Subject() = 0;//房间主题  
    virtual QString Description() = 0;//房间描述  
    virtual QDateTime Date() = 0;//房间建立时间  
    virtual QStringList Participants() = 0;//房间成员  
    virtual QString ParticipantId(const QString &szId) = 0;//成员ID  
    virtual QString ParticipantNick(const QString &szId) = 0;//成员呢称  
    virtual QIcon Icon();//房间图标  
    virtual bool IsProtracted() = 0;//是否是执久房间  
    virtual bool IsPrivate() = 0; //是否是私有  

    enum ENUM_Affiliation {
        UnspecifiedAffiliation,
        OutcastAffiliation,
        NoAffiliation,
        MemberAffiliation,
        AdminAffiliation,
        OwnerAffiliation,
    };
    virtual ENUM_Affiliation Affiliation(const QString &szId) = 0;//角色  

    virtual int Leave() = 0;
    virtual int Invitation(const QString &Id, const QString &reason) = 0;
    virtual int SendMessage(const QString &szMsg) = 0;
    virtual int Kick(const QString &szId, const QString &reason = QString()) = 0;

    virtual QSharedPointer<CMessage> GetMessage();
    
    enum Condition
    {
        Unknow = -2,
        NoError = -1,
        BadRequest,
        Conflict,
        FeatureNotImplemented,
        Forbidden,
        Gone,
        InternalServerError,
        ItemNotFound,
        JidMalformed,
        NotAcceptable,
        NotAllowed,
        NotAuthorized,
        PaymentRequired,
        RecipientUnavailable,
        Redirect,
        RegistrationRequired,
        RemoteServerNotFound,
        RemoteServerTimeout,
        ResourceConstraint,
        ServiceUnavailable,
        SubscriptionRequired,
        UndefinedCondition,
        UnexpectedRequest,
    };
    Condition Error();
protected:
    int SetError(Condition c);

signals:
    void sigParticipantAdd(const QString &szId);
    void sigParticipantRemoved(const QString &szId);
    void sigKick(const QString &szId, const QString &reason);

public slots:
private:
    QSharedPointer<CMessage> m_Message;
    Condition m_Condition;
};

#endif // GROUPCHAT_H
