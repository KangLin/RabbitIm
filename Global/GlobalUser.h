#ifndef GLOBALUSER_H
#define GLOBALUSER_H

#include <QObject>
#include <QSharedPointer>
#include "../UserInfo/UserInfoRoster.h"
#include "../UserInfo/UserInfoLocale.h"
#include "../UserInfo/COperateRoster.h"

class CGlobalUser : public QObject
{
    Q_OBJECT
public:
    explicit CGlobalUser(QObject *parent = 0);
    virtual ~CGlobalUser();

    int Init(QString szLocaleJid);
    int Clean();

    //更新本地用户信息  
    int UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid);
    int UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem);
    int UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid);

private:
    int LoadFromFile(QString szLocaleJid);
    int SaveToFile();
    int LoadLocaleFromFile(QString szLocaleJid);
    int SaveLocaleToFile();
    int LoadRosterFromFile(QString szLocaleJid);
    int SaveRosterToFile();

public:
    QSharedPointer<CUserInfoLocale> GetUserInfoLocale();
    int ProcessRoster(COperateRoster* pOperateRoster, void *para = NULL);
    QSharedPointer<CUserInfoRoster> GetUserInfoRoster(QString szBareJid);

private:
    //本地用户信息  
    QSharedPointer<CUserInfoLocale> m_UserInforLocale;
    //好友信息  
    QMap<QString, QSharedPointer<CUserInfoRoster> > m_UserInfoRoster;

private:
    bool m_bModify;//
signals:

public slots:

};

#endif // GLOBALUSER_H
