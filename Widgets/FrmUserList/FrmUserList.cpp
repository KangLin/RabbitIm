#include "FrmUserList.h"
#include "ui_FrmUserList.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"
#include "qxmpp/QXmppUtils.h"
#include "../../MainWindow.h"
#include <iostream>
#include <QKeyEvent>

CFrmUserList::CFrmUserList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);//这里不会产生内在泄漏，控件在romve操作时会自己释放内存。
    ui->tvUsers->setModel(m_pModel);
    ui->tvUsers->setHeaderHidden(true);
    ui->tvUsers->setExpandsOnDoubleClick(true);
    ui->tvUsers->setItemsExpandable(true);
    ui->tvUsers->installEventFilter(this);

    bool check = connect(ui->tvUsers, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(ui->tvUsers, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    m_Parent = (MainWindow*)parent;
    if(NULL == m_Parent->m_pClient)
        return;

    check = connect(m_Parent->m_pClient,
                         SIGNAL(presenceReceived(const QXmppPresence)),
                         SLOT(ChangedPresence(QXmppPresence)));
    Q_ASSERT(check);

    check = connect(&m_Parent->m_pClient->rosterManager(), SIGNAL(rosterReceived()),
                    SLOT(rosterReceived()));
    Q_ASSERT(check);

    check = connect(&m_Parent->m_pClient->rosterManager(), SIGNAL(itemAdded(QString)),
                    SLOT(itemAdded(QString)));
    Q_ASSERT(check);

    check = connect(&m_Parent->m_pClient->rosterManager(), SIGNAL(itemChanged(QString)),
                    SLOT(itemChanged(QString)));
    Q_ASSERT(check);

    check = connect(&m_Parent->m_pClient->rosterManager(), SIGNAL(itemRemoved(QString)),
                    SLOT(itemRemoved(QString)));
    Q_ASSERT(check);

    check = connect(&m_Parent->m_pClient->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)),
                    SLOT(vCardReceived(QXmppVCardIq)));
    Q_ASSERT(check);
}

CFrmUserList::~CFrmUserList()
{
    delete ui;
    QMap<QString, CRoster*>::iterator it;
    for(it = m_Rosters.begin(); it != m_Rosters.end(); it++)
    {
        delete it.value();
    }

    //TODO:删除组 m_Groups
}

int CFrmUserList::InsertUser(QXmppRosterIq::Item rosterItem)
{
    int nRet = 0;
    CRoster* pRoster = NULL;
    QMap<QString, CRoster*>::iterator itRosters;
    itRosters = m_Rosters.find(rosterItem.bareJid());
    if(m_Rosters.end() == itRosters)
    {
        pRoster = new CRoster(rosterItem.bareJid(), rosterItem.groups());
        m_Rosters.insert(pRoster->BareJid(), pRoster);
    }
    else
    {
        qFatal(qPrintable(tr("User had existed")));
        return -1;
    }

    QSet<QString> groups = rosterItem.groups();
    if(groups.isEmpty())
    {
        QString szDefaulGroup(tr("My friends"));
        groups.insert(szDefaulGroup);
    }

    for(QSet<QString>::iterator itGroup = groups.begin(); itGroup != groups.end(); itGroup++)
    {
        QString szGroup = *itGroup;
        QStandardItem* lstGroup = NULL;
        QMap<QString, QStandardItem*>::iterator it;
        it = m_Groups.find(szGroup);
        if(m_Groups.end() == it)
        {
            lstGroup = new QStandardItem(szGroup);
            lstGroup->setEditable(false);  //禁止双击编辑
            m_pModel->appendRow(lstGroup);
            m_Groups.insert(szGroup, lstGroup);
        }
        else
            lstGroup = it.value();

        lstGroup->appendRow(pRoster->GetItem());
        qDebug("CFrmUserList::InsertUser:%s (%s)",
               qPrintable(rosterItem.bareJid()),
               qPrintable(szGroup));
    }

    return nRet;
}

void CFrmUserList::itemAdded(const QString &bareJid)
{
    qDebug("CFrmUserList::itemAdded jid:%s", qPrintable(bareJid));
}

void CFrmUserList::itemChanged(const QString &bareJid)
{
    qDebug("CFrmUserList::itemChanged jid:%s", qPrintable(bareJid));
}

void CFrmUserList::itemRemoved(const QString &bareJid)
{
    qDebug("CFrmUserList::itemRemoved jid:%s", qPrintable(bareJid));
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(bareJid));
    if(m_Rosters.end() != it)
    {
        delete it.value();
        m_Rosters.remove(QXmppUtils::jidToBareJid(bareJid));
    }
}

//得到好友列表
void CFrmUserList::rosterReceived()
{
    qDebug("CFrmUserList:: Roster received");

    foreach (const QString &bareJid, m_Parent->m_pClient->rosterManager().getRosterBareJids())
    {
        InsertUser(m_Parent->m_pClient->rosterManager().getRosterEntry(bareJid));
        // TODOS:得到好友头像图片
        //m_Parent->m_pClient->vCardManager().requestVCard(bareJid);
    }
}

//好友状态改变
void CFrmUserList::ChangedPresence(const QXmppPresence &presence)
{
    qDebug("CFrmUserList::ChangedPresence jid:%s;status:%s",
           qPrintable(presence.from()),
           qPrintable(presence.statusText())
           );

    QString jid = QXmppUtils::jidToBareJid(presence.from());
    QMap<QString, CRoster*>::iterator it = m_Rosters.find(jid);
    if(m_Rosters.end() != it)
    {
        it.value()->ChangedPresence(presence.status().type());
    }
}

void CFrmUserList::vCardReceived(const QXmppVCardIq& vCard)
{
    QString bareJid = vCard.from();
    std::cout<<"CFrmUserList::vCardReceived:: vCard Received:: " << qPrintable(bareJid) <<std::endl;

    QString out("FullName: %1\nNickName: %2\n");
    std::cout<<qPrintable(out.arg(vCard.fullName()).arg(vCard.nickName())) <<std::endl;

    QString vCardsDir("vCards/");

    QDir dir;
    if(!dir.exists(vCardsDir))
        dir.mkdir(vCardsDir);

    QFile file("vCards/" + bareJid + ".xml");
    if(file.open(QIODevice::ReadWrite))
    {
        QXmlStreamWriter stream(&file);
        vCard.toXml(&stream);
        file.close();
        std::cout<<"CFrmUserList::vCardReceived:: vCard written to the file:: " << qPrintable(bareJid) <<std::endl;
    }

    QString name("vCards/" + bareJid + ".png");
    QByteArray photo = vCard.photo();
    QBuffer buffer;
    buffer.setData(photo);
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    QImage image = imageReader.read();
    if(image.save(name))
    {
        std::cout<<"CFrmUserList::vCardReceived:: Avatar saved to file" <<std::endl<<std::endl;
    }
}

void CFrmUserList::clicked(const QModelIndex &index)
{
    qDebug("CFrmUserList::clicked, row:%d; column:%d",
           index.row(), index.column());
    qDebug("CFrmUserList::clicked, %s",
           qPrintable(index.data().toString()));
}

void CFrmUserList::doubleClicked(const QModelIndex &index)
{
    qDebug("CFrmUserList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

    if(ui->tvUsers->isExpanded(index))
       ui->tvUsers->expand(index);
    else
       ui->tvUsers->collapse(index);
}

void CFrmUserList::resizeEvent(QResizeEvent *)
{
    //ui->tvUsers->move(0, 0);
    //ui->tvUsers->setGeometry(this->geometry());
}

bool CFrmUserList::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->tvUsers)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            qDebug("Users lists mouse button double chick");
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* key = (QKeyEvent*) event;
            qDebug("Users lists key press:%d", key->key());
        }
    }
    return false;
}
