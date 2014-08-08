#include "FrmGroupChat.h"
#include "ui_FrmGroupChat.h"

#define ROLE_JID Qt::UserRole + 1
#define ROLE_FORM_GROUPCHAT ROLE_JID + 1

CFrmGroupChat::CFrmGroupChat(QXmppMucRoom *room, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChat)
{
    ui->setupUi(this);
    Q_ASSERT(room);

    m_pRoom = room;
    
    bool check = connect(m_pRoom, SIGNAL(joined()),
                         SLOT(slotJoined()));
    Q_ASSERT(check);
}

CFrmGroupChat::~CFrmGroupChat()
{
    delete ui;
}

QStandardItem* CFrmGroupChat::GetItem()
{
    if(NULL == m_pItem)
        m_pItem = new QStandardItem(QIcon(":/icon/Conference"), m_pRoom->name());
    if(m_pItem)
    {
        m_pItem->setData(m_pRoom->jid(), ROLE_JID);
        QVariant v;
        v.setValue(this);
        m_pItem->setData(v, ROLE_FORM_GROUPCHAT);
    }
    return m_pItem;
}

void CFrmGroupChat::slotJoined()
{
    m_pItem->setData(m_pRoom->name(), Qt::DisplayRole);
}
