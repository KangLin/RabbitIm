#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QObject>
#include "qxmpp/QXmppDiscoveryIq.h"

class CConference : public QObject
{
    Q_OBJECT
public:
    enum REQUEST_TYPE{
        REQUEST_TYPE_NO,
        REQUEST_TYPE_SEVER,
        REQUEST_TYPE_ROOMS,
        REQUEST_TYPE_ROOMS_INFO,
        REQUEST_TYPE_Occupant
    };
    
    explicit CConference(QObject *parent = 0);
    virtual ~CConference();

    int Request(QString szJid = QString(), REQUEST_TYPE type = REQUEST_TYPE_NO);

signals:
    void sigFoundServer(const QString &szServerJid, const QString &szName);
    void sigFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms);
    void sigFoundRoomInfo(const QString& jid, const QXmppDataForm& form);
    void sigFoundOccupant(QString szOccupantJid);

public slots:
    void slotInfoReceived(const QXmppDiscoveryIq&iq);
    void slotItemsReceived(const QXmppDiscoveryIq&iq);

private:
    QString m_szJid;
    REQUEST_TYPE m_type;
    
};

#endif // CONFERENCE_H
