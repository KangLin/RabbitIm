/* 
 * 聊天室列表对话框  
 * 
 * 功能：
 * 1.启动时，从存储中加载保存的聊天室列表
 * 2.退出时，保存聊天室列表到存储中
 * 3.pc平台上双击列表条目时，打开相应聊天室窗口
 * 4.右击操作，显示新建聊天室、进入聊天室、退出聊天室、聊天室信息菜单，此菜单也加入到主菜单中的操作菜单中
 * 
 */

#ifndef FRMGROUPCHATLIST_H
#define FRMGROUPCHATLIST_H

#include <QFrame>
#include <QMenu>
#include <QStandardItemModel>
#include <QResizeEvent>
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
private:
    enum _GROUP_ROLE
    {
        GROUP_ITEM_ROLE_JID = Qt::UserRole + 1,
        GROUP_ITEM_ROLE_PROPERTIES = GROUP_ITEM_ROLE_JID + 1
    };
    enum _PROPERTIES
    {
        PROPERTIES_GROUP,
        PROPERTIES_ITEM,
        PROPERTIES_UNREAD_MESSAGE_COUNT
    };

private slots:
    /// This signal is emitted when an invitation to a chat room is received.
    //void slotInvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason);
    /// This signal is emitted when a new room is managed.
    //void slotRoomAdded(QXmppMucRoom *room);

    //成功加入聊天室时触发的消息  
    void slotJoinedGroup(const QString &szId);
    //成功离开聊天室时触发的消息  
    void slotLeave(const QString &szId);

    void slotUpdateMessage(const QString &szId);

private:
    int InitMenu();//初始化菜单  

    //菜单  
    QMenu m_Menu;
    QAction* m_pAction;
private slots:
    //增加菜单到主菜单的操作菜单下  
    void slotAddToMainMenu(QMenu* pMenu);
    //从主菜单中删除菜单  
    void slotRemoveFromMainMenu(QMenu* pMenu);
    //菜单更新时触发  
    void slotUpdateMenu();
    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽 
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);

    //建立群组菜单  
    void on_actionCreate_chat_room_triggered();
    //加入群组菜单  
    void on_actionJoin_chat_room_triggered();
    //打开群组菜单  
    void on_actionOpen_chat_room_triggered();
    //离开群组菜单  
    void on_actionLeave_room_triggered();

private:
    //列表控件  
    CCustomTreeView m_GroupList;
    QStandardItemModel *m_pModel; //好友列表树型控件  
    
    int ItemRemove(const QString &szId);

private slots:
    //树形列表控件响应事件  
    void slotClicked(const QModelIndex & index);
    void slotDoubleClicked(const QModelIndex & index);

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent*);

    /**
     * @brief 得到当前选中房间  
     *
     * @return QString 如果选中，则返回房间 ID，否则返回空  
     */
    QString GetCurrentRoom();

private:
    Ui::CFrmGroupChatList *ui;
};

#endif // FRMGROUPCHATLIST_H
