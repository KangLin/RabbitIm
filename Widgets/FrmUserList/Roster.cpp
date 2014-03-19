#include "Roster.h"

CRoster::CRoster(QObject *parent) :
    QObject(parent)
{
}

CRoster::CRoster(QString jid)
{
    SetJid(jid);
}

CRoster::CRoster(QString jid, QSet<QString> groups)
{
    SetJid(jid);
    SetGroups(groups);
}

CRoster::~CRoster()
{
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        if(p->parent())
        {
            p->parent()->removeRow(p->row()); //控件会自己释放 QStandardItem 内存
        }
    }
}

QString CRoster::Name()
{
    return QXmppUtils::jidToUser(m_szJid);
}

QString CRoster::BareJid()
{
    return QXmppUtils::jidToBareJid(m_szJid);
}

QString CRoster::Domain()
{
    return QXmppUtils::jidToDomain(m_szJid);
}

QString CRoster::Resouce()
{
    return QXmppUtils::jidToResource(m_szJid);
}

int CRoster::SetJid(QString jid)
{
    m_szJid = jid;
    return 0;
}

QSet<QString> CRoster::Groups()
{
    return m_Groups;
}

int CRoster::SetGroups(const QSet<QString> &groups)
{
    m_Groups = groups;
}

QStandardItem* CRoster::GetItem()
{
    QStandardItem* pItem = new QStandardItem(Name());
    QVariant v;
    v.setValue(this);
    pItem->setData(v);
    pItem->setEditable(true);//允许双击编辑
    m_lstUserListItem.push_back(pItem);
    return pItem;
}

QString CRoster::GetStatusText(QXmppPresence::Status status)
{
    if(QXmppPresence::Status::Online == status.type())
        return tr("OnLine");
    else if(QXmppPresence::Status::Away == status.type())
        return tr("Temporarily away");
    else if(QXmppPresence::Status::Chat == status.type())
        return tr("Chat");
    else if(QXmppPresence::Status::DND == status.type())
        return tr("Do not disturb");
    else if(QXmppPresence::Status::Invisible == status.type())
        return tr("Invisible");
    else if(QXmppPresence::Status::XA == status.type())
        return tr("Away for an extended period");
}

QColor CRoster::GetStatusColor(QXmppPresence::Status status)
{
    if(QXmppPresence::Status::Online == status.type())
        return QColor(0, 255, 0);
    else if(QXmppPresence::Status::Away == status.type())
        return QColor(0, 0, 255);
    else if(QXmppPresence::Status::Chat == status.type())
        return QColor(0, 255, 0);
    else if(QXmppPresence::Status::DND == status.type())
        return QColor(255, 0, 0);
    else if(QXmppPresence::Status::Invisible == status.type())
        return QColor(255, 255, 255);
    else if(QXmppPresence::Status::XA == status.type())
        return QColor(255, 0, 255);
}

//TODO:修改成图片表示
int CRoster::ChangedPresence(QXmppPresence::Status::Type status)
{
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        p->setData(GetStatusColor(status), Qt::BackgroundRole);
    }
    return 0;
}

