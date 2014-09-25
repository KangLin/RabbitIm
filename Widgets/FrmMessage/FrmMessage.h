#ifndef FRMMESSAGE_H
#define FRMMESSAGE_H

#include <QFrame>
#include <QStandardItemModel>
#include <QMenu>
#include "../FrmCustom/MenuMessageSend.h"
#include "UserInfo/UserInfo.h"

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

    int AppendMessage(const QString &szMessage);
    int AppendMessageToList(const QString &szMessage, 
                            const QString &bareJid,
                            const QString &name,
                            bool bRemote = false);

protected:
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent* );
    virtual void closeEvent(QCloseEvent *e);
    virtual void changeEvent(QEvent*);
    
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void on_pbBack_clicked();
    void on_pbSend_clicked();
    void on_tbMore_clicked();
    void on_pbVideo_clicked();
    void on_lbAvatar_clicked();//点击头像显示用户详细信息对话框  

    void ChangedPresence(CUserInfo::USER_INFO_STATUS status);
/*
    void slotSendFileTriggered();//20140712文件发送  
    void slotShotScreenTriggered();//20140712截屏  
    */
private:
    int Init(const QString &szId = QString());

private:
    Ui::CFrmMessage *ui;

    QSharedPointer<CUserInfo> m_User;
    QMenu m_MoreMenu;//功能按钮菜单  
    CMenuMessageSend m_MessageSendMenu;
};

#endif // FRMMESSAGE_H
