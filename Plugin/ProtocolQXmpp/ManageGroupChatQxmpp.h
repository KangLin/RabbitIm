#ifndef MANAGEGROUPCHATQXMPP_H
#define MANAGEGROUPCHATQXMPP_H

#include <QObject>
#include <QMap>
#include "GroupChatQxmpp.h"
#include "Widgets/FrmGroupChat/ManageGroupChat.h"

class CManageGroupChatQxmpp : public CManageGroupChat
{
    Q_OBJECT
public:
    explicit CManageGroupChatQxmpp(QObject *parent = 0);
    /**
     * @brief 登录后初始化  
     *
     * @param szId
     * @return int
     */
    virtual int Init(const QString& szId);
    virtual int Clean();
    virtual int Create(const QString &szName,
                       const QString &szSubject,
                       const QString &szPassword = QString(),
                       const QString &szDescription = QString(),
                       bool bProtracted = false,
                       bool bPrivated = false,
                       const QString &szNick = QString());
    virtual int Join(const QString &szId, const QString &szPassword = QString(), const QString &szNick = QString());
    virtual QSharedPointer<CGroupChat> Get(const QString &szId);
    virtual bool IsJoined(const QString &szId);

signals:

private slots:
    void slotInvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason);

private:
    QSharedPointer<CGroupChatQxmpp> Join1(const QString &szId, const QString &szPassword, const QString &szNick);
};

#endif // MANAGEGROUPCHATQXMPP_H
