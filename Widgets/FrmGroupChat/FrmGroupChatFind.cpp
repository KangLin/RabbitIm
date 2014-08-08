#include "FrmGroupChatFind.h"
#include "ui_FrmGroupChatFind.h"
#include "../../Global.h"
#include "qxmpp/QXmppDiscoveryManager.h"

CFrmGroupChatFind::CFrmGroupChatFind(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatFind)
{
    ui->setupUi(this);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pModel = new QStandardItemModel(this);//这里不会产生内在泄漏，控件在romve操作时会自己释放内存。  
    if(m_pModel)
    {
        ui->treeView->setModel(m_pModel);
    }

    m_pListModel = new QStandardItemModel(this);
    if(m_pListModel)
        ui->listView->setModel(m_pListModel);

    bool check = connect(&m_Conference, SIGNAL(sigFoundServer(QString,QString)), 
                         SLOT(slotFoundServer(QString,QString)));
    Q_ASSERT(check);

    check = connect(&m_Conference, SIGNAL(sigFoundRoom(QList<QXmppDiscoveryIq::Item>)),
                    SLOT(slotFoundRoom(QList<QXmppDiscoveryIq::Item>)));
    Q_ASSERT(check);
}

CFrmGroupChatFind::~CFrmGroupChatFind()
{
    delete ui;
}

void CFrmGroupChatFind::closeEvent(QCloseEvent *)
{
    deleteLater();
}


void CFrmGroupChatFind::on_pbJoin_clicked()
{
    close();
}

void CFrmGroupChatFind::on_pbCancel_clicked()
{
    close();
}

void CFrmGroupChatFind::on_pbRefresh_clicked()
{
    m_pModel->clear();
    m_Conference.Request();
}

void CFrmGroupChatFind::on_treeView_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    m_pListModel->clear();
    QVariant v = m->data(index, Qt::UserRole + 1);
    QString jid = v.value<QString>();
    m_Conference.Request(jid, CConference::REQUEST_TYPE_ROOMS);
}

void CFrmGroupChatFind::slotFoundServer(const QString& jid, const QString& name)
{
     QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Server"), name);
     pItem->setData(jid);
     m_pModel->appendRow(pItem);
}

void CFrmGroupChatFind::slotFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms)
{
    QXmppDiscoveryIq::Item item;
    foreach(item, Rooms)
    {
        QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Conference"), item.name());
        pItem->setData(item.jid());
        m_pListModel->appendRow(pItem);
    }
}
