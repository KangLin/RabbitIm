#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppCallManager.h"

class CXmppClient : public QXmppClient
{
    Q_OBJECT

public:
    CXmppClient(QObject *parent = 0);
    ~CXmppClient();

    QXmppCallManager m_CallManager;
};

#endif // XMPPCLIENT_H
