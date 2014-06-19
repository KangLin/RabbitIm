#include "FrmUserList.h"
#include "ui_FrmUserList.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"
#include "qxmpp/QXmppUtils.h"
#include "../../MainWindow.h"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include "Roster.h"

CFrmUserList::CFrmUserList(QWidget *parent) :
    QFrame(parent),
    m_UserList(parent),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);

    InitMenu();

    m_pModel = new QStandardItemModel(this);//这里不会产生内在泄漏，控件在romve操作时会自己释放内存。
    //设置背景
    //m_UserList.setStyleSheet(g_Global.GetStyleSheet());
    m_UserList.setModel(m_pModel);
    m_UserList.setHeaderHidden(true);
    m_UserList.setExpandsOnDoubleClick(true);
    m_UserList.setItemsExpandable(true);
    m_UserList.show();

    InsertGroup(tr("My friends"));
    
    bool check = connect(&m_UserList, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);


    m_pMainWindow = (MainWindow*)parent;
    if(NULL == m_pMainWindow->m_pClient)
        return;

    check = connect(m_pMainWindow->m_pClient,
                         SIGNAL(presenceReceived(const QXmppPresence)),
                         SLOT(ChangedPresence(QXmppPresence)));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->rosterManager(), SIGNAL(rosterReceived()),
                    SLOT(rosterReceived()));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->rosterManager(), SIGNAL(subscriptionReceived(QString)),
                    SLOT(subscriptionReceived(QString)));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->rosterManager(), SIGNAL(itemAdded(QString)),
                    SLOT(itemAdded(QString)));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->rosterManager(), SIGNAL(itemChanged(QString)),
                    SLOT(itemChanged(QString)));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->rosterManager(), SIGNAL(itemRemoved(QString)),
                    SLOT(itemRemoved(QString)));
    Q_ASSERT(check);

    check = connect(&m_pMainWindow->m_pClient->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)),
                    SLOT(vCardReceived(QXmppVCardIq)));
    Q_ASSERT(check);

    check = connect(m_pMainWindow->m_pClient, SIGNAL(messageReceived(QXmppMessage)),
                    SLOT(clientMessageReceived(QXmppMessage)));
    Q_ASSERT(check);
}

CFrmUserList::~CFrmUserList()
{
    QMap<QString, CRoster*>::iterator it;
    for(it = m_Rosters.begin(); it != m_Rosters.end(); it++)
    {
        delete it.value();
    }

    //删除组 m_Groups，不需要删，列表控件析构时会自己删除

    if(m_pMenu)
        delete m_pMenu;

    delete ui;   
}

int CFrmUserList::InitMenu()
{
    bool check = true;
    m_pMenu = new QMenu(tr("Operator roster(&O)"), this);
    if(!m_pMenu)
    {
        LOG_MODEL_WARNING("Roster",  "CFrmUserList::InitMenu new QMenu fail");
        return -1;
    }

    check = connect(m_pMenu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    //菜单设置
    m_pMenuAction = NULL;
    m_pMenu->addAction(ui->actionAddRoster_A);
    check = connect(ui->actionAddRoster_A, SIGNAL(triggered()),
                    SLOT(slotAddRoster()));
    Q_ASSERT(check);

    m_pMenu->addAction(ui->actionRemoveRoster_R);
    check = connect(ui->actionRemoveRoster_R, SIGNAL(triggered()),
                    SLOT(slotRemoveRoster()));
    Q_ASSERT(check);

    m_pMenu->addAction(ui->actionAgreeAddRoster);
    check = connect(ui->actionAgreeAddRoster, SIGNAL(triggered()),
                    SLOT(slotAgreeAddRoster()));
    Q_ASSERT(check);
    return 0;
}

int CFrmUserList::EnableAllActioins(bool bEnable)
{
    EnableAction(ui->actionAddRoster_A, bEnable);
    EnableAction(ui->actionAgreeAddRoster, bEnable);
    EnableAction(ui->actionRemoveRoster_R, bEnable);
    return 0;
}

int CFrmUserList::EnableAction(QAction *pAction, bool bEnable)
{
    pAction->setEnabled(bEnable);
    pAction->setVisible(bEnable);
    return 0;
}

int CFrmUserList::AddToMainMenu(QMenu *pMenu, QAction* pAction)
{
    m_pMenuAction = pMenu->insertMenu(pAction, m_pMenu);
    return 0;
}

int CFrmUserList::DeleteFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pMenuAction);
    return 0;
}

void CFrmUserList::slotUpdateMenu()
{
    EnableAllActioins(false);
    //如果是组上,显示增加好友
    EnableAction(ui->actionAddRoster_A);

    //判断是在组上还是在好友上:
    CRoster* p = GetCurrentRoster();
    if(p)
    {
        //增加订阅
        if(QXmppRosterIq::Item::None == p->GetSubScriptionType()
             || QXmppRosterIq::Item::From == p->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);

        //如果是好友上,显示删除好友
        EnableAction(ui->actionRemoveRoster_R);

        //TODO: 查看好友信息
        //TODO: 移动到组

    }
    else
    {
        //TODO:新建组
    }
    return;
}

void CFrmUserList::slotCustomContextMenuRequested(const QPoint &pos)
{
    m_pMenu->exec(QCursor::pos());
}

void CFrmUserList::slotAddRoster()
{
    QSet<QString> groups;
    groups = GetGroupsName();

    m_frmAddRoster.Init(m_pMainWindow->m_pClient, groups);
    m_frmAddRoster.show();
}

void CFrmUserList::slotAgreeAddRoster()
{
    CRoster* p = GetCurrentRoster();
    if(p)
        m_pMainWindow->m_pClient->rosterManager().subscribe(p->BareJid());
}

void CFrmUserList::slotRemoveRoster()
{
    CRoster* p = GetCurrentRoster();
    if(p)
        m_pMainWindow->m_pClient->rosterManager().removeItem(p->BareJid());
}

void CFrmUserList::clientMessageReceived(const QXmppMessage &message)
{
    LOG_MODEL_DEBUG("Roster", "MainWindow:: message Received:type:%d;state:%d;from:%s;to:%s;body:%s",
           message.type(),
           message.state(), //消息的状态 0:消息内容，其它值表示这个消息的状态
           qPrintable(message.from()),
           qPrintable(message.to()),
           qPrintable(message.body())
          );

    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(message.from()));
    if(m_Rosters.end() != it)
    {
        if(QXmppMessage::None == message.state())
            it.value()->AppendMessage(message.body());
        //TODO:消息输入状态显示
    }
}

QStandardItem*  CFrmUserList::InsertGroup(QString szGroup)
{
     QStandardItem* lstGroup = NULL;
    lstGroup = new QStandardItem(szGroup);
    lstGroup->setEditable(false);  //禁止双击编辑
    m_pModel->appendRow(lstGroup);
    m_Groups.insert(szGroup, lstGroup);
    return lstGroup;
}

int CFrmUserList::UpdateGroup(CRoster* pRoster, QSet<QString> groups)
{
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
            //新建立组条目
            /*lstGroup = new QStandardItem(szGroup);
            lstGroup->setEditable(false);  //禁止双击编辑
            m_pModel->appendRow(lstGroup);
            m_Groups.insert(szGroup, lstGroup);//*/
            lstGroup = InsertGroup(szGroup);
        }
        else
            lstGroup = it.value();

        lstGroup->appendRow(pRoster->GetItem());
        LOG_MODEL_DEBUG("Roster", "CFrmUserList::UpdateGroup:%s,(%s)",
                qPrintable(pRoster->BareJid()),
                qPrintable(szGroup));
    }

    return 0;
}

int CFrmUserList::InsertUser(QXmppRosterIq::Item rosterItem)
{
    int nRet = 0;
    CRoster* pRoster = NULL;
    QMap<QString, CRoster*>::iterator itRosters;
    itRosters = m_Rosters.find(rosterItem.bareJid());
    if(m_Rosters.end() == itRosters)
    {
        //新建好友对象实例
        pRoster = new CRoster(rosterItem,
                              this->m_pMainWindow);
        m_Rosters.insert(pRoster->BareJid(), pRoster);
    }
    else
    {
        LOG_MODEL_ERROR("Roster", qPrintable("Error:User had existed"));
        return -1;
    }

    nRet = UpdateGroup(pRoster, rosterItem.groups());

    return nRet;
}

void CFrmUserList::subscriptionReceived(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::subscriptionReceived:%s", qPrintable(bareJid));
    m_frmAddRoster.Init(m_pMainWindow->m_pClient, GetGroupsName(), bareJid);
    m_frmAddRoster.show();
    m_frmAddRoster.activateWindow();
}

void CFrmUserList::itemAdded(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemAdded jid:%s", qPrintable(bareJid));
    QXmppRosterIq::Item item = m_pMainWindow->m_pClient->rosterManager().getRosterEntry(bareJid);
    InsertUser(item);
}

void CFrmUserList::itemChanged(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemChanged jid:%s", qPrintable(bareJid));
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(bareJid));
    if(m_Rosters.end() != it)
    {
        CRoster* pRoster = it.value();

        QXmppRosterIq::Item item = m_pMainWindow->m_pClient->rosterManager().getRosterEntry(bareJid);
        pRoster->UpdateItems(item);
        UpdateGroup(pRoster, item.groups());
    }
}

void CFrmUserList::itemRemoved(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemRemoved jid:%s", qPrintable(bareJid));
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
    LOG_MODEL_DEBUG("Roster", "CFrmUserList:: Roster received");

    foreach (const QString &bareJid, m_pMainWindow->m_pClient->rosterManager().getRosterBareJids())
    {
        QXmppRosterIq::Item item = m_pMainWindow->m_pClient->rosterManager().getRosterEntry(bareJid);
        InsertUser(item);
        // TODOS:得到好友头像图片
        //m_pMainWindow->m_pClient->vCardManager().requestVCard(bareJid);
    }
}

//好友出席状态改变
void CFrmUserList::ChangedPresence(const QXmppPresence &presence)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::ChangedPresence jid:%s;status:%s",
           qPrintable(presence.from()),
           qPrintable(presence.statusText())
           );

    QString jid = QXmppUtils::jidToBareJid(presence.from());
    QMap<QString, CRoster*>::iterator it = m_Rosters.find(jid);
    if(m_Rosters.end() != it)
    {
        it.value()->ChangedPresence(presence.availableStatusType());
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
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::clicked, row:%d; column:%d",
           index.row(), index.column());

    if(m_UserList.isExpanded(index))
       m_UserList.collapse(index);
    else
       m_UserList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //TODO:暂时根据是否有根节点来判断是组还是好友
    if(m->parent(index).isValid())
    {
        //是用户结点，打开消息对话框
        QVariant v = m->data(index, Qt::UserRole + 1);
        if(v.canConvert<CRoster*>())
        {
            CRoster* p = v.value<CRoster*>();
            p->ShowMessageDialog();
        }
    }
}

void CFrmUserList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());
}

void CFrmUserList::resizeEvent(QResizeEvent *)
{
    m_UserList.setGeometry(this->geometry());
}

QSet<QString> CFrmUserList::GetGroupsName()
{
    QSet<QString> groups;
    QMap<QString, QStandardItem*>::iterator it;
    for(it = m_Groups.begin(); it != m_Groups.end(); it++)
        groups << it.key();
    return groups;
}

CRoster* CFrmUserList::GetCurrentRoster()
{
    QModelIndex index = m_UserList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return NULL;

    //TODO:暂时根据是否有根节点来判断是组还是好友
    if(m->parent(index).isValid())
    {
        //是用户结点，删除它
        QVariant v = m->data(index, Qt::UserRole + 1);
        if(v.canConvert<CRoster*>())
        {
            return v.value<CRoster*>();
        }
    }
    return NULL;
}
