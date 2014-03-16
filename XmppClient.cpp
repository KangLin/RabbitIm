#include "XmppClient.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"

CXmppClient::CXmppClient(QObject *parent)
    : QXmppClient(parent)
{
}

CXmppClient::~CXmppClient()
{
}
