#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"

class CXmppClient : public QXmppClient
{
    Q_OBJECT

public:
    CXmppClient(QObject *parent = 0);
    ~CXmppClient();

};

#endif // XMPPCLIENT_H
