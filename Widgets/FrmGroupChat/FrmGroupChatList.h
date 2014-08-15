/* 
 * 聊天室列表对话框  
 */

#ifndef FRMGROUPCHATLIST_H
#define FRMGROUPCHATLIST_H

#include <QFrame>
#include <QMenu>
#include <QStandardItemModel>
#include <QResizeEvent>
#include "../../XmppClient.h"
#include "../FrmCustom/CustomTreeView.h"
#include "FrmGroupChat.h"

namespace Ui {
class CFrmGroupChatList;
}

class CFrmGroupChatList : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmGroupChatList(QWidget *parent = 0);
    ~CFrmGroupChatList();

private slots:
    /// This signal is emitted when an invitation to a chat room is received.
    void slotInvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason);
    /// This signal is emitted when a new room is managed.
    void slotRoomAdded(QXmppMucRoom *room);

    //成功加入聊天室时触发的消息  
    void slotJoinedGroup(const QString &jid, CFrmGroupChat* pChat);
    //成功离开聊天室时触发的消息  
    void slotLeft(const QString &jid, CFrmGroupChat* pChat);
    
    //查找群组菜单  
    void slotActionFindGroup();
    //离开聊天室菜单  
    void slotActionLeave();
    //增加菜单到主菜单的操作菜单下  
    void slotAddToMainMenu(QMenu* pMenu);
    //从主菜单中删除菜单  
    void slotRemoveFromMainMenu(QMenu* pMenu);
    //菜单更新时触发  
    void slotUpdateMenu();
    //主菜单更新进触发  
    void slotUpdateMainMenu();

    
    //树形列表控件响应事件 
    void slotClicked(const QModelIndex & index);
    void slotDoubleClicked(const QModelIndex & index);

    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽 
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);
private:
    int InitMenu();

    void resizeEvent(QResizeEvent *e);

private:
    Ui::CFrmGroupChatList *ui;

    //菜单  
    QMenu m_Menu;
    QAction* m_pAction;

    //列表控件  
    CCustomTreeView m_GroupList;
    QStandardItemModel *m_pModel;           //好友列表树型控件  

    //聊天室列表<聊天室jid, CFrmGroupChat*>  
    //CFmGroupChat* 在这里进行管理  
    QMap<QString, CFrmGroupChat*> m_Group;
};

#endif // FRMGROUPCHATLIST_H
