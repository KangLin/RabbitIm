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

    m_pTreeModel = new CItemModelCustom(this);
    ui->treeView->setModel(m_pTreeModel);
    ui->treeView->setSelectionMode(QAbstractItemView::MultiSelection);
    m_pTreeModel->SetSelectionModel(ui->treeView->selectionModel());
    GLOBAL_USER->ProcessRoster(this);
    m_GroupChat = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(m_GroupChat.isNull())
        return;
    ui->lbTitle->setText(tr("Inviter rosters to join %1").arg(m_GroupChat->ShowName()));
}

CDlgInviter::~CDlgInviter()
{
    if(m_pTreeModel)
        delete m_pTreeModel;
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
    pItem->setData(info->GetId(), USERLIST_ITEM_ROLE_ID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    pItem->setEditable(false);//禁止条目编辑  
    pItem->setCheckable(true);//设置复选框可用  
    pItem->setSelectable(true);//设置可选中  

#ifdef DEBUG
    pItem->setToolTip(info->GetId());
#endif 

    //改变item图标  
    //pItem->setData(QIcon(QPixmap::fromImage(info->GetPhoto())), Qt::DecorationRole);
    pItem->setIcon(QIcon(QPixmap::fromImage(info->GetPhoto())));


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
    lstGroup->setData("", USERLIST_ITEM_ROLE_ID);
    lstGroup->setTristate(true);  //设置三态复选框状态  
    lstGroup->setCheckable(true); //设置复选框可用  
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

void CDlgInviter::on_buttonBox_accepted()
{
    QItemSelectionModel* pMode = ui->treeView->selectionModel();
    foreach(QModelIndex index, pMode->selectedIndexes())
    {
        QStandardItem* pItem = m_pTreeModel->itemFromIndex(index);
        if(!pItem)
            continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_GROUP)
            continue;
        QString szId = pItem->data(USERLIST_ITEM_ROLE_ID).value<QString>();
        if(m_GroupChat.isNull())
            return;
        m_GroupChat->Invitation(szId, ui->txtReason->text());
    }
}
