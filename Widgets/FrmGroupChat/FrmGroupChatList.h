#ifndef FRMGROUPCHATLIST_H
#define FRMGROUPCHATLIST_H

#include <QFrame>
#include <QMenu>
#include "../../XmppClient.h"

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

    void slotFindGroup();
    void slotAddToMainMenu(QMenu* pMenu);
    void slotRemoveFromMainMenu(QMenu* pMenu);
    void slotUpdateMenu();

private:
    int InitMenu();
    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再做个槽联接这个事件   
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void contextMenuEvent(QContextMenuEvent * event);

private:
    Ui::CFrmGroupChatList *ui;
    QMenu m_Menu;
    QAction* m_pAction;
};

#endif // FRMGROUPCHATLIST_H
