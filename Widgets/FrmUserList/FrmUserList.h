#ifndef FRMUSERLIST_H
#define FRMUSERLIST_H

#include <QFrame>
#include <QString>
#include <QStandardItemModel>
#include <QMap>
#include <QMenu>
#include <QResizeEvent>
#include "qxmpp/QXmppRosterIq.h"
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppMessage.h"
#include "../FrmAddRoster/FrmAddRoster.h"
#include "../FrmCustom/CustomTreeView.h"

class CRoster;

namespace Ui {
class CFrmUserList;
}

class CFrmMain;
class MainWindow;
class CFrmUserList : public QFrame
{
    Q_OBJECT

    friend class CFrmMain;

public:
    explicit CFrmUserList(QWidget *parent = 0);
    ~CFrmUserList();

public:
    //TODO:以后放在未读消息中维护  
    //显示最后一个消息窗口  
    int ShowMessageDialog();

public slots:
    //更新好友  
    void slotRefresh();

private slots:
    //显示上下文件菜单(右键菜单)  
    //控件contextMenuPolicy属性要设置为CustomContextMenu，才能触customContextMenuRequested事件，再联接这个槽 
    //如果设置为DefaultContextMenu，则触发右键菜单事件 void contextMenuEvent(QContextMenuEvent * event);  
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotUpdateMenu();//更新菜单  
    void slotUpdateMainMenu();//主菜单更新  
    //把好友列表菜单加到主菜单中,调用者不需要用此菜单时，负责调用DeleteFromMainMenu释放  
    void slotAddToMainMenu(QMenu* pMenu);
    //把好友列表菜单从主菜单中移除  
    void slotDeleteFromMainMenu(QMenu *pMenu);

private:
    int InitMenu();//初始化菜单  
    int EnableAllActioins(bool bEnable = true);
    int EnableAction(QAction* pAction, bool bEnable = true);
    QMenu m_Menu;      //好友列表操作菜单  
    QAction* m_pMenuAction;//用于存储m_Menu位于主菜单中的位置  

private:
    //根据好友jid，得到相应的 CRoster 对象  
    CRoster* GetRoster(QString szJid);

    //向用户列表中插入用户  
    int InsertUser(QXmppRosterIq::Item rosterItem);
    //在组队列中插入组  
    QStandardItem*  InsertGroup(QString szGroup);
    //更新组中用户  
    int UpdateGroup(CRoster* pRoster, QSet<QString> groups);

    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent* e);

    //得到树形控件当前选中的好友指针  
    //如果为空,则为组  
    CRoster *GetCurrentRoster();

    QSet<QString> GetGroupsName();

protected slots:
    //好友列表  
    void slotRosterReceived();
    //好友出席状态改变时触发  
    void slotChangedPresence(const QXmppPresence &presence);
    //得到好友详细信息时触发  
    void slotvCardReceived(const QXmppVCardIq&);

    //当有好友请求订阅时触发，在这个函数中调用acceptSubscription()接受请求，  
    //请用refuseSubscription()拒绝请求  
    //如果设置QXmppConfiguration::autoAcceptSubscriptions(),则QXmpp直接接受请求,不触发本信号  
    void slotSubscriptionReceived(const QString& bareJid);
    //当一个好友实体订阅时触发(即好友增加)  
    void slotItemAdded(const QString& bareJid);
    // 好友实体订阅发生改变时触发(即好友关系发生改变)  
    void slotItemChanged(const QString& bareJid);
    //删除订阅时触发(即删除好友)  
    void slotItemRemoved(const QString& bareJid);

    //接收好友消息  
    void slotClientMessageReceived(const QXmppMessage &message);

    //树形列表控件响应事件 
    void clicked(const QModelIndex & index);
    void doubleClicked(const QModelIndex & index);

private slots:
    //增加好友订阅  
    void slotAddRoster();
    //从好友列表中同间增加此好友订阅  
    void slotAgreeAddRoster();
    //删掉好友订阅  
    void slotRemoveRoster();
    //显示好友详细信息  
    void slotInformationRoster();

private:
    Ui::CFrmUserList *ui;

    CCustomTreeView m_UserList;
    QStandardItemModel *m_pModel;           //好友列表树型控件   
    MainWindow *m_pMainWindow;
    QMap<QString, CRoster*> m_Rosters;      //好友列表:<bareJic, CRoster>  
    QMap<QString, QStandardItem*> m_Groups; //组列表:<组名,QStandardItem>  
    QString m_LastUser;                     //接收的最后一个消息用户  
    CFrmAddRoster m_frmAddRoster;           //增加好友对话框  

};

#endif // FRMUSERLIST_H
