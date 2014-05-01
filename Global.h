#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppClient.h"

class CGlobal : public QObject
{
    Q_OBJECT
public:
    explicit CGlobal(QObject *parent = 0);
    virtual ~CGlobal();

    //得到本地用户住息
    int SetJid(QString jid);
    QString GetName();
    QString GetBareJid();
    QString GetDomain();
    QString GetResource();
    QColor GetUserColor();
    QColor GetRosterColor();
    int SetUserColor(QColor &color);
    int SetRosterColor(QColor &color);

    QString GetXmppServerHost();
    int SetXmppServerHost(QString &host);
    int SetXmppServerPort(qint16 port);
    qint16 GetXmppServerPort();
    QString GetXmppServer();
    int SetXmppServer(QString server);
    QString GetStunServer();
    int SetStunServer(QString &server);
    qint16 GetStunServerPort();
    int SetStunServerPort(qint16 &port);
    QString GetTurnServer();
    int SetTurnServer(QString &server);
    qint16 GetTurnServerPort();
    int SetTurnServerPort(qint16 &port);
    QString GetTurnServerUser();
    int SetTurnServerUser(QString &user);
    QString GetTurnServerPassword();
    int SetTurnServerPassword(QString &password);

    //好友状态文本表示
    QString GetStatusText(QXmppPresence::AvailableStatusType status);
    //好友状态颜色表示
    QColor GetStatusColor(QXmppPresence::AvailableStatusType status);

    //用户数据存放目录
    QString GetDirUserData(const QString bareJid = QString());
    //应用程序目录
    QString GetDirApplication();
    //应用程序配置目录
    QString GetDirApplicationConfigure();

signals:

public slots:

private:
    QString m_szLocalJid;
    QColor m_UserColor;   //本地用户颜色
    QColor m_RosterColor; //好友颜色

    QString m_szXmppServerHost; //Xmpp服务器IP地址
    int m_szXmppServerPort;
    QString m_szXmppServer; //Xmpp服务器地址
    QString m_szTurnServer; //Turn服务器地址
    int m_nTurnServerPort;
    QString m_szTurnUser;
    QString m_szTurnPassword;
    QString m_szStunServer; //Stun服务器地址
    int m_nStunServerPort;
};

extern CGlobal g_Global;
#endif // GLOBAL_H
