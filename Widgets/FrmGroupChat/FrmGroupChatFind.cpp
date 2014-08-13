#include "FrmGroupChatFind.h"
#include "ui_FrmGroupChatFind.h"
#include "../../Global.h"
#include "qxmpp/QXmppDiscoveryManager.h"
#include "../FrmUserList/Roster.h"
#include <QMessageBox>
#include "../../MainWindow.h"
#include "FrmCreateGroupChatRoom.h"

CFrmGroupChatFind::CFrmGroupChatFind(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatFind)
{
    ui->setupUi(this);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    InitMenu();

    m_pModel = new QStandardItemModel(this);//这里不会产生内在泄漏，控件在romve操作时会自己释放内存。  
    if(m_pModel)
    {
        ui->treeView->setModel(m_pModel);
    }

    m_pListModel = new QStandardItemModel(this);
    if(m_pListModel)
    {
        ui->listView->setModel(m_pListModel);
    }

    bool check = connect(&m_Conference, SIGNAL(sigFoundServer(QString,QString)), 
                         SLOT(slotFoundServer(QString,QString)));
    Q_ASSERT(check);

    check = connect(&m_Conference, SIGNAL(sigFoundRoom(QList<QXmppDiscoveryIq::Item>)),
                    SLOT(slotFoundRoom(QList<QXmppDiscoveryIq::Item>)));
    Q_ASSERT(check);

    check = connect(&m_Conference, SIGNAL(sigFoundRoomInfo(QString,QXmppDataForm)),
                    SLOT(slotFoundRoomInfo(QString,QXmppDataForm)));
    Q_ASSERT(check);
}

CFrmGroupChatFind::~CFrmGroupChatFind()
{
    delete ui;
}

CFrmGroupChatFind* CFrmGroupChatFind::Instance()
{
    static CFrmGroupChatFind* pFind = NULL;
    if(!pFind)
        pFind = new CFrmGroupChatFind();
    return pFind;
}

void CFrmGroupChatFind::showEvent(QShowEvent *)
{
    CGlobal::Instance()->GetMainWindow()->setEnabled(false);
    on_pbRefresh_clicked();
}

void CFrmGroupChatFind::closeEvent(QCloseEvent *)
{
    CGlobal::Instance()->GetMainWindow()->setEnabled(true);
}

void CFrmGroupChatFind::on_pbJoin_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::critical(this, tr("Join room error"),
                              tr("Don't select room, please select room."));
        return;
    }
    QVariant v = index.model()->data(index, ROLE_JID);
    QString szJid = v.value<QString>();

    CFrmGroupChat* pGroupChat = new CFrmGroupChat();
    if(pGroupChat)
    {
        bool check = connect(pGroupChat, SIGNAL(sigJoined(const QString&,CFrmGroupChat*)),
                             SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)));
        Q_ASSERT(check);

        if(!pGroupChat->Join(szJid))
            delete pGroupChat;
    }

    close();
}

void CFrmGroupChatFind::on_pbCancel_clicked()
{
    close();
}

void CFrmGroupChatFind::on_pbRefresh_clicked()
{
    m_pModel->clear();
    m_Conference.Request(QString(), CConference::REQUEST_TYPE_SEVER);
}

void CFrmGroupChatFind::on_treeView_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    m_pListModel->clear();
    QVariant v = m->data(index, ROLE_JID);
    QString jid = v.value<QString>();
    m_Conference.Request(jid, CConference::REQUEST_TYPE_ROOMS);
}

void CFrmGroupChatFind::slotFoundServer(const QString& jid, const QString& name)
{
     QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Server"), name);
     pItem->setData(jid, ROLE_JID);
     pItem->setToolTip(jid);
     m_pModel->appendRow(pItem);
}

void CFrmGroupChatFind::slotFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms)
{
    QXmppDiscoveryIq::Item item;
    foreach(item, Rooms)
    {
        QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Conference"), item.name());
        pItem->setData(item.jid(), ROLE_JID);
        pItem->setToolTip(item.jid());
        m_pListModel->appendRow(pItem);
        m_Conference.Request(item.jid(), CConference::REQUEST_TYPE_ROOMS_INFO);
    }
}

void CFrmGroupChatFind::slotFoundRoomInfo(const QString &jid, const QXmppDataForm &form)
{
}

void CFrmGroupChatFind::on_listView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    m_Conference.Request();
    on_pbJoin_clicked();
}

void CFrmGroupChatFind::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        m_Menu.exec(QCursor::pos());
    }
}

int CFrmGroupChatFind::InitMenu()
{
    m_Menu.addAction(QIcon(":/icon/Add"), tr("New room"), this, SLOT(slotNewRoom()));
    return 0;
}

void CFrmGroupChatFind::slotUpdateMenu()
{
}

void CFrmGroupChatFind::slotNewRoom()
{
    m_Conference.Request();
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QVariant v = index.model()->data(index, ROLE_JID);
        QString szJid = v.value<QString>();
        CFrmCreateGroupChatRoom* pRoom = CFrmCreateGroupChatRoom::Instance(szJid);
        bool check = connect(pRoom, SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)),
                    SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)));
        Q_ASSERT(check);
        pRoom->show();
    }
}
