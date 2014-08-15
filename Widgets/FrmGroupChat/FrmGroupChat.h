#ifndef FRMGROUPCHAT_H
#define FRMGROUPCHAT_H

#include <QFrame>
#include <QStandardItem>
#include "qxmpp/QXmppMucManager.h"
#include "../FrmCustom/MenuMessageSend.h"

namespace Ui {
class CFrmGroupChat;
}

class CFrmGroupChat : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmGroupChat(QWidget *parent = 0);
    ~CFrmGroupChat();

    enum ENUM_ROLE{
        ROLE_GROUPCHAT_JID = Qt::UserRole + 1,
        ROLE_GROUPCHAT_OBJECT = ROLE_GROUPCHAT_JID + 1
    };
    QList<QStandardItem *> GetItem();
    bool Join(const QString &jid);  //加入聊天室  
    bool Create(const QString &jid);//新建聊天室  
    bool Leave(const QString &message = QString());//离开聊天室  
    bool setConfiguration(const QXmppDataForm &form);//设置聊天室配置  

signals:
    /// 当成功加入聊天室时触发  
    void sigJoined(const QString &jid, CFrmGroupChat* pChat);
    /// 当未成功加入或离开聊天时触发  
    void sigLeft(const QString &jid, CFrmGroupChat* pChat);

private slots:
    /// 当成功加入房间后会触发此消息  
    void slotJoined();
    /// 当离开房间或加入房间失败时，会触发这个消息  
    void slotLeft();
    void slotNameChanged(const QString &name);
    /// This signal is emitted when an error is encountered.
    void slotError(const QXmppStanza::Error &error);
    /// 允许动作改变时触发这个信号  
    void slotAllowedActionsChanged(QXmppMucRoom::Actions actions) const;
    /// 当从房间接收到配置信息时触发  
    void slotConfigurationReceived(const QXmppDataForm &configuration);
    /// 当房间的主题改变时触发  
    void slotSubjectChanged(const QString &subject);
    /// 当一个消息接收时触发  
    void slotMessageReceived(const QXmppMessage &message);
    /// 当一个成员加入房间时触发  
    void slotParticipantAdded(const QString &jid);
    /// 当一个成员改变时触发  
    void slotParticipantChanged(const QString &jid);
    /// 当一个参与者离开房间时触发  
    void slotParticipantRemoved(const QString &jid);
    /// 当权限接收时触发  
    void slotPermissionsReceived(const QList<QXmppMucItem> &permissions);
    
    void on_pbSend_clicked();
    void on_pbClose_clicked();
    void on_pbMember_clicked();
    void on_lstMembers_clicked(const QModelIndex &index);
    void on_lstMembers_doubleClicked(const QModelIndex &index);
    
private:
    //消息加入到消息列表  
    int AppendMessageToList(const QString &szMessage, const QString &nick);
    //改变标题  
    int ChangeTitle();

    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *e);
    //用于过滤发送消息键盘事件  
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::CFrmGroupChat *ui;

    QXmppMucRoom* m_pRoom;
    QStandardItemModel *m_pModelMembers; //聊天室成员列表控件  

    QList<QStandardItem*> m_ListItems;   //聊天室条目列表  
    QStandardItem* m_pItem;              //聊天室条目  
    QStandardItem* m_pItemMessageCount;  //聊天室未读消息记数条目  
    int m_MessageCount;                  //未读消息记数  

    QXmppDataForm m_DataForm;    //聊天室配置信息,用于新建聊天室  

    CMenuMessageSend m_MessageSendMenu;//发送消息按键配置菜单  
};

#endif // FRMGROUPCHAT_H
