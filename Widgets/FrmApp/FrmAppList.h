#ifndef FRMAPPLIST_H
#define FRMAPPLIST_H

#include <QWidget>
#include <QMenu>
#include <QStandardItemModel>
#include "../FrmCustom/CustomTreeView.h"
#include "Plugin/PluginApp.h"

namespace Ui {
class CFrmAppList;
}

class CFrmAppList : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmAppList(QWidget *parent = 0);
    ~CFrmAppList();

private slots:
    //树形列表控件点击事件 
    void clicked(const QModelIndex & index);
    //树形列表控件双击事件  
    void doubleClicked(const QModelIndex & index);
    
private:
    QMenu m_Menu;      //列表操作菜单  
    int InitMenu();
private slots:
    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽  
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);
    //打开应用  
    void slotOpenApp();   
    void slotCloseApp();
    void slotAboutApp();
    void slotCollectApp();

private:
    QStandardItem* ItemInsertGroup(QString szGroup);
    int InitList();
    int InsertApp(QSharedPointer<CPluginApp> app);
    int InsertAppInGroup(QStandardItem* pGroupItem, QSharedPointer<CPluginApp> app);
    
private:
    Ui::CFrmAppList *ui;

    enum _USERLIST_ROLE
    {
        ROLE_GROUP = Qt::UserRole + 1,
        ROLE_APPLICATION = ROLE_GROUP + 1
    };
    
    CCustomTreeView m_AppList;
    QStandardItemModel *m_pModel;           //好友列表树型控件

};

#endif // FRMAPPLIST_H
