/**
 * @brief 完成树型控件有复选框时的选择功能  
 * 使用：
 *  CItemModelCustom* pTreeModel = new CItemModelCustom(this);
 *  QTreeView treeView;
 *  treeView.setModel(pTreeModel);
 *  treeView.setSelectionMode(QAbstractItemView::MultiSelection); //设置多选  
 *  pTreeModel->SetSelectionModel(treeView.selectionModel());
 *
 */

#ifndef ITEMMODELCUSTOM_H
#define ITEMMODELCUSTOM_H

#include <QObject>
#include <QStandardItemModel>
#include <QItemSelectionModel>

class CItemModelCustom : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CItemModelCustom(QObject *parent = 0);
    int SetSelectionModel(QItemSelectionModel* pModel);

private slots :
    void slotTreeItemChanged(QStandardItem* pItem);
    //void slotCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
private:
    void treeItem_checkAllChild(QStandardItem* pItem, bool bCheck);
    void treeItem_CheckChildChanged(QStandardItem* pItem);
    Qt::CheckState checkSibling(QStandardItem* pItem);

    QItemSelectionModel* m_pSelectionModel;
    int m_nCount;//记数器，用于防止信号递归调用  
};

#endif // ITEMMODELCUSTOM_H
