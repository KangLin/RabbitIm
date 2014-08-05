#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppCallManager.h"
#include "qxmpp/QXmppTransferManager.h"
#include "qxmpp/QXmppMucManager.h"

class CXmppClient : public QXmppClient
{
    Q_OBJECT

public:
    CXmppClient(QObject *parent = 0);
    ~CXmppClient();

    QXmppCallManager m_CallManager;
    QXmppMucManager m_MucManager;
    QXmppTransferManager m_TransferManager;
};

#endif // XMPPCLIENT_H
