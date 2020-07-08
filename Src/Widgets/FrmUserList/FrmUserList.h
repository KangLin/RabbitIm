#ifndef FRMUSERLIST_H
#define FRMUSERLIST_H

#include <QFrame>
#include <QString>
#include <QStandardItemModel>
#include <QMap>
#include <QMenu>
#include <QResizeEvent>
#include "../DlgAddRoster/DlgAddRoster.h"
#include "../FrmCustom/CustomTreeView.h"
#include "UserInfo/COperateRoster.h"
#include "Client/Client.h"

namespace Ui {
class CFrmUserList;
}

class CFrmUserList : public QWidget, COperateRoster
{
    Q_OBJECT

public:
    explicit CFrmUserList(QWidget *parent = 0);
    ~CFrmUserList();

public:
    /**
     * @brief 处理好友，COperateRoster::ProcessRoster 重载函数  
     *
     * @param roster：要处理的好友  
     * @param para：详见 enum _OPERATOR_TYPE  
     * @return int：成功返回0，失败返回非0，枚举将停止。  
     */
    virtual int ProcessRoster(QSharedPointer<CUserInfo> roster, void *para = NULL);

private slots:
    //更新好友  
    void slotRefresh();

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
    //在组队列中插入组  
    QStandardItem* ItemInsertGroup(QString szGroup);
    //更新组中用户  
    int ItemUpdateGroup(QSharedPointer<CUserInfo> info);
    //插入好友条目  
    int ItemInsertRoster(const QString &szId);
    //更新好友条目  
    int ItemUpdateRoster(const QString &szId);
    //删除好友条目  
    int ItemRemoveRoster(const QString &szId);
    //生成好友条目  
    QList<QStandardItem *> NewItemRoster(QSharedPointer<CUserInfo> info);

    //事件  
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent* e);

    //得到当前树型控件中选中的条目  
    //如果是好友，则返回好友的barejid  
    //否则返回空QString  
    QString GetCurrentRoster();
    QSet<QString> GetGroupsName();

private slots:
    //好友列表从文件中加载完成  
    void slotLoadRosterFromStorage();
    //更新好友信息  
    void slotUpdateRosterUserInfo(const QString &szId,  QSharedPointer<CUser> userInfo);
    //删除好友信息  
    void slotRemoveRosterUserInfo(const QString &szId);
    //好友出席状态改变时触发  
    void SlotChangedStatus(const QString& szId);
    //消息更新  
    void slotMessageUpdate(const QString& szId);

    //菜单槽  
    //增加好友菜单槽  
    void slotAddRoster();
    //重命名好友名称  
    void slotRenameRoster();
    //移动好友组  
    void slotMoveRoster();
    //删掉好友菜单槽  
    void slotRemoveRoster();
    //同意增加好友菜单槽  
    void slotAgreeAddRoster();
    //更新好友列表  
    void slotRefreshRosterList();
    //显示好友详细信息槽  
    void slotInformationRoster();
    //删除组槽  
    void slotRemoveGroup();
    
    void slotSendMessage();
    void slotSendFile();
    void slotCallVideo();
    void slotCallAudio();
    void slotMonitor();

    //当有好友请求订阅时触发，在这个函数中调用acceptSubscription()接受请求，  
    //请用refuseSubscription()拒绝请求  
    //如果设置QXmppConfiguration::autoAcceptSubscriptions(),则QXmpp直接接受请求,不触发本信号  
    void slotRosterAddReceived(const QString& szId, const CClient::SUBSCRIBE_TYPE & type);

    //树形列表控件点击事件 
    void clicked(const QModelIndex & index);
    //树形列表控件双击事件  
    void doubleClicked(const QModelIndex & index);

    void slotItemChanged(QStandardItem *item);
    void slotEntered(const QModelIndex &index);

    QString GetNodeStateFile();
    //从存储中加载组节点的状态  
    int LoadGroupNodeStateFromStorage();
    //保存组节点状态到存储中  
    int SaveGroupNodeStateToStorage();

private:
    Ui::CFrmUserList *ui;
    //传给 ProcessRoster 中的参数类型  
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
        PROPERTIES_ROSTER,
        PROPERTIES_UNREAD_MESSAGE_COUNT
    };

    CCustomTreeView m_UserList;
    QStandardItemModel *m_pModel;           //好友列表树型控件   

    QMap<QString, QStandardItem*> m_Groups; //组列表:<组名,QStandardItem>  
};

#endif // FRMUSERLIST_H
