#ifndef GLOBALUSER_H
#define GLOBALUSER_H

#include <QObject>
#include <QSharedPointer>
#include "../UserInfo/UserInfoRoster.h"
#include "../UserInfo/UserInfoLocale.h"

class CGlobalUser : public QObject
{
    Q_OBJECT
public:
    explicit CGlobalUser(QObject *parent = 0);
    virtual ~CGlobalUser();

    int Init(QString szLocaleJid);
    int Clean();
    
    //更新本地用户信息  
    int UpdateLocaleUserInfo(const QXmppVCardIq &vCard, QString jid);

private:
    int LoadFromFile(QString szLocaleJid);
    int SaveToFile();

public:
    QSharedPointer<CUserInfoLocale> GetUserInfoLocale();
    
private:
    QSharedPointer<CUserInfoLocale> m_UserInforLocale;//本地用户信息  
    //TODO:好友信息  
    
    
private:
    bool m_bModify;//
signals:
    
public slots:

};

#endif // GLOBALUSER_H
