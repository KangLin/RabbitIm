#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>

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

signals:

public slots:

private:
    QString m_szLocalJid;
    QColor m_UserColor;   //本地用户颜色
    QColor m_RosterColor; //好友颜色

    QString m_szXmppServerHost; //Xmpp服务器IP地址
    QString m_szXmppServer; //Xmpp服务器地址
    QString m_szTurnServer; //Turn服务器地址
    int m_nTurnServerPort;
    QString m_szTurnUser;
    QString m_szTurnPassword;
    QString m_szStunServer; //Stun服务器地址
    int m_nStunServerPort;
};

#endif // GLOBAL_H
