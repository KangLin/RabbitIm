#ifndef FrmRecentMsgList_H
#define FrmRecentMsgList_H

#include <QFrame>
#include <QString>
#include <QStandardItemModel>
#include <QMap>
#include <QList>
#include <QResizeEvent>
#include <QMutex>
#include "CustomTreeView.h"
#include "UserInfo/User.h"
#include <QMenu>

namespace Ui {
class CFrmRecentMsgList;
}

class CFrmRecentMsgList : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRecentMsgList(QWidget *parent = 0);
    ~CFrmRecentMsgList();

private slots:
    void slotMessageUpdate(const QString& szId);
    void LoadFromStorage();
    //好友出席状态改变时触发  
    void SlotChangedStatus(const QString& szId);
    void slotRefresh();
    void slotRemove(const QString &szId);

private slots:
    //树形列表控件响应事件 
    void clicked(const QModelIndex & index);
    void doubleClicked(const QModelIndex & index);

private:
    int InitMenu();
    QMenu m_Menu;      //好友列表操作菜单  
    QAction* m_pMenuAction;//用于存储m_Menu位于主菜单中的位置  
private slots:
    //把好友列表菜单加到主菜单中,调用者不需要用此菜单时，负责调用DeleteFromMainMenu释放  
    void slotAddToMainMenu(QMenu* pMenu);
    //把好友列表菜单从主菜单中移除  
    void slotDeleteFromMainMenu(QMenu *pMenu);
    void slotUpdateMainMenu();//主菜单更新  

    void slotOpenMessage();
    void slotRemoveMessage();
    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽 
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent* e);

    enum _OPERATOR_TYPE
    {
        OPERATE_TYPE_INSERT_ROSTER,
        OPERATE_TYPE_UPDATE_ROSTER
    };

    enum _USERLIST_ROLE
    {
        USERLIST_ITEM_ROLE_JID = Qt::UserRole + 1,
        USERLIST_ITEM_ROLE_PROPERTIES = USERLIST_ITEM_ROLE_JID + 1
    };

    enum _PROPERTIES
    {
        PROPERTIES_GROUP,
        PROPERTIES_ID,
        PROPERTIES_UNREAD_MESSAGE_COUNT
    };

    int InsertItem(const QString &szId,  int nRow = 0);
    int RemoveItem(const QString &szId);
    int UpdateItem(const QString &szId);

private:
    Ui::CFrmRecentMsgList *ui;

    CCustomTreeView m_MsgList;
    QStandardItemModel *m_pModel;       //列表树型控件  
};

#endif // FrmRecentMsgList_H
