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

    QString GetXmppServer();
    int SetXmppServer(QString server);
    QString GetStunServer();
    int SetStunServer(QString &server);
    int GetStunServerPort();
    int SetStunServerPort(int &port);
    QString GetTurnServer();
    int SetTurnServer(QString &server);
    int GetTurnServerPort();
    int SetTurnServerPort(int &port);

signals:

public slots:

private:
    QString m_szLocalJid;
    QColor m_UserColor;   //本地用户颜色
    QColor m_RosterColor; //好友颜色

    QString m_szXmppServer; //Xmpp服务器地址
    QString m_szTurnServer; //Turn服务器地址
    int m_nTurnServerPort;
    QString m_szStunServer; //Stun服务器地址
    int m_nStunServerPort;
};

#endif // GLOBAL_H
