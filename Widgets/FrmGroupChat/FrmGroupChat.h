#ifndef FRMGROUPCHAT_H
#define FRMGROUPCHAT_H

#include <QFrame>
#include <QStandardItem>
#include "../FrmCustom/MenuMessageSend.h"
#include "GroupChat.h"

namespace Ui {
class CFrmGroupChat;
}

class CFrmGroupChat : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmGroupChat(const QString &szId, QWidget *parent = 0);
    ~CFrmGroupChat();

    enum ENUM_ROLE{
        ROLE_GROUPCHAT_JID = Qt::UserRole + 1,
        ROLE_GROUPCHAT_OBJECT = ROLE_GROUPCHAT_JID + 1
    };
signals:
    /**
     * @brief 窗口关闭时通知父窗口删除自己  
     *
     * @param obj:些窗口对象指针  
     */
    void sigClose(QFrame* obj);

private slots:
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
    void changeEvent(QEvent*);
    //用于过滤发送消息键盘事件  
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::CFrmGroupChat *ui;

    QSharedPointer<CGroupChat> m_Room;
    QStandardItemModel *m_pModelMembers; //聊天室成员列表控件  

    CMenuMessageSend m_MessageSendMenu;//发送消息按键配置菜单  
};

#endif // FRMGROUPCHAT_H
