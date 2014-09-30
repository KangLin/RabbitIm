#ifndef FRMMESSAGE_H
#define FRMMESSAGE_H

#include <QFrame>
#include <QStandardItemModel>
#include <QMenu>
#include "../FrmCustom/MenuMessageSend.h"
#include "UserInfo/User.h"
#include "Message/ChatActions/ChatAction.h"

class MainWindow;

namespace Ui {
class CFrmMessage;
}

class CFrmMessage : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmMessage(QWidget *parent = 0, Qt::WindowFlags f = 0);
    explicit CFrmMessage(const QString &szId,  QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~CFrmMessage();

signals:
    /**
     * @brief 窗口关闭时通知父窗口删除自己  
     *
     * @param obj:些窗口对象指针  
     */
    void sigClose(QFrame* obj);

private:
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent* );
    virtual void closeEvent(QCloseEvent *e);
    virtual void changeEvent(QEvent*);

    bool eventFilter(QObject *target, QEvent *event);

    int AppendMessageToOutputView(std::vector<QSharedPointer<CChatAction> > action);

private slots:
    void on_pbBack_clicked();
    void on_pbSend_clicked();
    void on_pbEmoticons_clicked();
    void on_tbMore_clicked();
    void on_pbVideo_clicked();
    void on_lbAvatar_clicked();//点击头像显示用户详细信息对话框  

    void ChangedPresence(CUserInfo::USER_INFO_STATUS status);
    //刷新控件  
    void slotRefresh();
    void slotMessageUpdate(const QString& szId);
    void slotUpdateRoster(const QString& szId, QSharedPointer<CUser> user);
    void slotAnchorClicked(const QUrl &url);
    void slotEmoteInsertRequested(const QString &s);
/*
    void slotSendFileTriggered();//20140712文件发送  
    void slotShotScreenTriggered();//20140712截屏  
    */

private:
    int Init(const QString &szId = QString());

private:
    Ui::CFrmMessage *ui;

    QSharedPointer<CUser> m_User;
    QMenu m_MoreMenu;//功能按钮菜单  
    CMenuMessageSend m_MessageSendMenu;
};

#endif // FRMMESSAGE_H
