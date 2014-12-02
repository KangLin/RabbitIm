#include "DlgInviter.h"
#include "ui_DlgInviter.h"
#include "Global/Global.h"
#include <QDesktopWidget>

CDlgInviter::CDlgInviter(const QString &szId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgInviter)
{
    ui->setupUi(this);
    
    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->geometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif
    
    m_pTreeModel = new QStandardItemModel(this);
    m_pListModel = new QStandardItemModel(this);
    ui->treeView->setModel(m_pTreeModel);
    ui->listView->setModel(m_pListModel);
    GLOBAL_USER->ProcessRoster(this);
}

CDlgInviter::~CDlgInviter()
{
    if(m_pTreeModel)
        delete m_pTreeModel;
    if(m_pListModel)
        delete m_pListModel;
    delete ui;
}

int CDlgInviter::ItemInsertRoster(const QString& szId)
{
    int nRet = 0;
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return -1;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();
    //呢称条目  
    QStandardItem* pItem = new QStandardItem(info->GetShowName());
    pItem->setData(info->GetId(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    pItem->setEditable(false);
    pItem->setCheckable(true);

#ifdef DEBUG
    pItem->setToolTip(info->GetId());
#endif 

    //改变item图标  
    pItem->setData(QIcon(QPixmap::fromImage(info->GetPhoto())), Qt::DecorationRole);


    ItemUpdateGroup(pItem, info->GetGroups());
    return nRet;
}

int CDlgInviter::ItemUpdateGroup(QStandardItem *pItems, QSet<QString> groups)
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
            lstGroup = ItemInsertGroup(szGroup);
        }
        else
            lstGroup = it.value();

        lstGroup->appendRow(pItems);
    }

    return 0;
}

QStandardItem*  CDlgInviter::ItemInsertGroup(QString szGroup)
{
    QStandardItem* lstGroup = NULL;
    lstGroup = new QStandardItem(szGroup);
    lstGroup->setEditable(false);  //禁止双击编辑 
    lstGroup->setData(PROPERTIES_GROUP, USERLIST_ITEM_ROLE_PROPERTIES);
    lstGroup->setData("", USERLIST_ITEM_ROLE_JID);
    lstGroup->setTristate(true);
    lstGroup->setCheckable(true);
    m_pTreeModel->appendRow(lstGroup);
    m_Groups.insert(szGroup, lstGroup);
    return lstGroup;
}

int CDlgInviter::ProcessRoster(QSharedPointer<CUserInfo> roster, void *para)
{
    int nRet = 0;

    nRet = ItemInsertRoster(roster->GetId());

    return nRet;
}
