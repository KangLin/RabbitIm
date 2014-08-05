#include "XmppClient.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"

CXmppClient::CXmppClient(QObject *parent)
    : QXmppClient(parent)
{
    this->addExtension(&m_CallManager);
    this->addExtension(&m_MucManager);
    this->addExtension(&m_TransferManager);
}

CXmppClient::~CXmppClient()
{
}
