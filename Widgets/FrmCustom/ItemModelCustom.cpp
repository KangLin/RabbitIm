#include "ItemModelCustom.h"
#include "Global/Global.h"

CItemModelCustom::CItemModelCustom(QObject *parent) :
    QStandardItemModel(parent)
{
    m_pSelectionModel = NULL;
    m_nCount = 0;
    m_bSignal = true;

    //关联项目属性改变的信号和槽  
    bool check = connect(this, SIGNAL(itemChanged(QStandardItem*)),
                         SLOT(slotTreeItemChanged(QStandardItem*)));
    Q_ASSERT(check);
}

int CItemModelCustom::SetSelectionModel(QItemSelectionModel *pModel)
{
    if(!pModel)
        return -1;
    m_pSelectionModel = pModel;
    bool check = false;
    /*check = connect(m_pSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotCurrentChanged(QModelIndex,QModelIndex)));
    Q_ASSERT(check);*/
    check = connect(m_pSelectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    Q_ASSERT(check);
    return 0;
}

void CItemModelCustom::slotTreeItemChanged(QStandardItem *pItem)
{
    if(!pItem)
        return;

    if(!pItem->isCheckable())
        return;

    //检查记数器，防止信号递归调用  
    if(m_nCount > 0)
        return;
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::slotTreeItemChanged:m_nCount:%d", m_nCount);
    m_nCount++;
    m_bSignal = false;//禁止选择信号,防止递归调用  

    if(pItem->isTristate())//节点是三态复选框  
    {
        Qt::CheckState state = pItem->checkState();
        if(Qt::Unchecked == state)
        {
            //当前是全部未选中状态，需要对其子项目进行全未选  
            treeItem_checkAllChild(pItem, false);
            if(m_pSelectionModel)
                m_pSelectionModel->select(this->indexFromItem(pItem), QItemSelectionModel::Deselect);
        }
        else
        {
            //当前是部分选中状态,或全选中状态，需要对其子项目进行全选  
            treeItem_checkAllChild(pItem, true);
            if(m_pSelectionModel)
                m_pSelectionModel->select(this->indexFromItem(pItem), QItemSelectionModel::Select);
        }
    }
    else //节点是复选框，对父节点操作  
    {
        treeItem_CheckChildChanged(pItem);
        if(m_pSelectionModel)
            m_pSelectionModel->select(this->indexFromItem(pItem), 
                                      Qt::Checked == pItem->checkState()
                                      ? QItemSelectionModel::Select
                                      : QItemSelectionModel::Deselect);
    }

    m_bSignal = true;
    m_nCount--;
}

///
/// \brief 递归设置所有的子项目为全选或全不选状态  
/// \param item 当前项目  
/// \param check true时为全选，false时全不选  
///
void CItemModelCustom::treeItem_checkAllChild(QStandardItem * pItem, bool bCheck)
{
    if(!pItem)
        return;
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::treeItem_checkAllChild");
    int rowCount = pItem->rowCount();//得到条目的子条目数  
    for(int i = 0; i < rowCount; ++i)
    {
        QStandardItem* childItems = pItem->child(i);
        treeItem_checkAllChild(childItems,bCheck);
    }
    if(pItem->isCheckable())
    {
        pItem->setCheckState(bCheck ? Qt::Checked : Qt::Unchecked);
        
        if(m_pSelectionModel)
            m_pSelectionModel->select(this->indexFromItem(pItem), 
                                      bCheck ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
    }
}

///
/// \brief 根据子节点的改变，更改父节点的选择情况  
/// \param item
///
void CItemModelCustom::treeItem_CheckChildChanged(QStandardItem* pItem)
{
    if(nullptr == pItem)
        return;
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::treeItem_CheckChildChanged");
    Qt::CheckState siblingState = checkSibling(pItem);
    QStandardItem* parentItem = pItem->parent();
    if(nullptr == parentItem)
        return;
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable() && parentItem->isTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
    if(m_pSelectionModel)
        m_pSelectionModel->select(this->indexFromItem(parentItem), 
                                  Qt::Unchecked != parentItem->checkState()
                                  ? QItemSelectionModel::Select
                                  : QItemSelectionModel::Deselect);
}

///
/// \brief 测量兄弟节点的情况，如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
/// \param item
/// \return 如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
///
Qt::CheckState CItemModelCustom::checkSibling(QStandardItem* pItem)
{
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::checkSibling");
    //先通过父节点获取兄弟节点
    QStandardItem * parent = pItem->parent();
    if(nullptr == parent)
        return pItem->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount = 0, unCheckedCount = 0;
    Qt::CheckState state;
    for(int i = 0; i < brotherCount; ++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}
/*
void CItemModelCustom::slotCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::slotCurrentChanged:current.row:%d;previous.row:%d", current.row(), previous.row());
    QStandardItem* pItem = this->itemFromIndex(current);
    if(pItem->isCheckable())
        pItem->setCheckState(pItem->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
}*/

void CItemModelCustom::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!m_bSignal)
        return;
    LOG_MODEL_DEBUG("CItemModelCustom", "CItemModelCustom::slotCurrentChanged:selected:%d;deselected:%d", selected.size(), deselected.size());
    QModelIndexList indexs = selected.indexes();
    foreach(QModelIndex index, indexs)
    {
        QStandardItem* pItem = this->itemFromIndex(index);
        if(pItem->isCheckable())
            pItem->setCheckState(Qt::Checked);
    }

    indexs = deselected.indexes();
    foreach(QModelIndex index, indexs)
    {
        QStandardItem* pItem = this->itemFromIndex(index);
        if(pItem->isCheckable())
            pItem->setCheckState(Qt::Unchecked);
    }
}
