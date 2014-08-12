#ifndef FRMGROUPCHAT_H
#define FRMGROUPCHAT_H

#include <QFrame>
#include <QStandardItem>
#include "qxmpp/QXmppMucManager.h"

namespace Ui {
class CFrmGroupChat;
}

class CFrmGroupChat : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmGroupChat(const QString &jid, QWidget *parent = 0);
    ~CFrmGroupChat();

    enum ENUM_ROLE{
        ROLE_GROUPCHAT_JID = Qt::UserRole + 1,
        ROLE_GROUPCHAT_OBJECT = ROLE_GROUPCHAT_JID + 1
    };
    QStandardItem* GetItem();

signals:
    void sigJoined(const QString &jid, CFrmGroupChat* pChat);
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

private:
    int AppendMessageToList(const QString &szMessage, const QString &nick);
    int ChangeTitle();

private:
    Ui::CFrmGroupChat *ui;

    QXmppMucRoom* m_pRoom;
    QStandardItem* m_pItem;

    QStandardItemModel *m_pModel;  //好友列表控件  
};

#endif // FRMGROUPCHAT_H
