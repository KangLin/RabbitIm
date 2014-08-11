#ifndef FRMGROUPCHATLIST_H
#define FRMGROUPCHATLIST_H

#include <QFrame>
#include <QMenu>
#include <QStandardItemModel>
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

    void slotJoinGroup(const QString &jid);
    void slotFindGroup();
    void slotAddToMainMenu(QMenu* pMenu);
    void slotRemoveFromMainMenu(QMenu* pMenu);
    void slotUpdateMenu();

    
    //树形列表控件响应事件 
    void slotClicked(const QModelIndex & index);
    void slotDoubleClicked(const QModelIndex & index);

    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽 
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);
private:
    int InitMenu();

    void resizeEvent(QResizeEvent *);

private:
    Ui::CFrmGroupChatList *ui;
    QMenu m_Menu;
    QAction* m_pAction;

    CCustomTreeView m_GroupList;
    QStandardItemModel *m_pModel;           //好友列表树型控件  
    QMap<QString, CFrmGroupChat*> m_Group;
};

#endif // FRMGROUPCHATLIST_H
