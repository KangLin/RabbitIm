#include "Conference.h"
#include "../../Global/Global.h"
#include "qxmpp/QXmppDiscoveryManager.h"

CConference::CConference(QObject *parent) :
    QObject(parent)
{
    m_type = REQUEST_TYPE_NO;
    bool check = false;
    QXmppDiscoveryManager* pDis = GET_CLIENT->findExtension<QXmppDiscoveryManager>();
    check = connect(pDis, SIGNAL(infoReceived(QXmppDiscoveryIq)),
                    SLOT(slotInfoReceived(QXmppDiscoveryIq)));
    Q_ASSERT(check);
    
    check = connect(pDis, SIGNAL(itemsReceived(QXmppDiscoveryIq)),
                    SLOT(slotItemsReceived(QXmppDiscoveryIq)));
    Q_ASSERT(check);
}

CConference::~CConference()
{
    QXmppDiscoveryManager* pDis = XMPP_CLIENT->findExtension<QXmppDiscoveryManager>();
    pDis->disconnect(this);
}

void CConference::slotInfoReceived(const QXmppDiscoveryIq &iq)
{
    LOG_MODEL_DEBUG("group chat", "CConference::slotInfoReceived:");

    QXmppDiscoveryIq::Identity id;
    switch(m_type) {
    case REQUEST_TYPE_NO:
        return;
    case REQUEST_TYPE_SEVER:
        foreach(id, iq.identities())
        {
            LOG_MODEL_DEBUG("group chat", "identity:category:%s;name:%s;type:%s;language:%s",
                            id.category().toStdString().c_str(),
                            id.name().toStdString().c_str(),
                            id.type().toStdString().c_str(),
                            id.language().toStdString().c_str());
            if(id.category() == "conference")
                emit sigFoundServer(iq.from(), id.name());
        }
        break;
    case REQUEST_TYPE_ROOMS_INFO:
        foreach(id, iq.identities())
        {
            LOG_MODEL_DEBUG("group chat", "REQUEST_TYPE_ROOMS_INFO:identity:category:%s;name:%s;type:%s;language:%s",
                            id.category().toStdString().c_str(),
                            id.name().toStdString().c_str(),
                            id.type().toStdString().c_str(),
                            id.language().toStdString().c_str());
        }

        break;
    default:
        break;
    }
}

void CConference::slotItemsReceived(const QXmppDiscoveryIq &iq)
{
    LOG_MODEL_DEBUG("group chat", "CConference::slotItemsReceived:");

    QXmppDiscoveryIq::Item item;
    switch (m_type) {
    case REQUEST_TYPE_NO:
        return;
    case REQUEST_TYPE_SEVER:
        foreach(item, iq.items())
        {
            LOG_MODEL_DEBUG("group chat", "item:jid:%s;name:%s;node:%s",
                            item.jid().toStdString().c_str(),
                            item.name().toStdString().c_str(), 
                            item.node().toStdString().c_str());
            XMPP_CLIENT->findExtension<QXmppDiscoveryManager>()->requestInfo(item.jid());
        }
        break;
    case REQUEST_TYPE_ROOMS:
        emit sigFoundRoom(iq.items());
        break;
    default:
        break;
    }
}

int CConference::Request(QString szJid, REQUEST_TYPE type)
{
    QXmppDiscoveryManager *dis = XMPP_CLIENT->findExtension<QXmppDiscoveryManager>();
    m_type = type;
    switch(type)
    {
    case REQUEST_TYPE_SEVER:
        m_szJid = USER_INFO_LOCALE->GetDomain();
        break;
    case REQUEST_TYPE_ROOMS_INFO:
        m_szJid = szJid;
        dis->requestInfo(m_szJid);
        return 0;
    case REQUEST_TYPE_ROOMS:
    case REQUEST_TYPE_Occupant:
    default:
        m_szJid = szJid;
    }

    dis->requestItems(m_szJid);

    return 0;
}
