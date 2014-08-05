#ifndef FRMMESSAGE_H
#define FRMMESSAGE_H

#include <QFrame>
#include <QStandardItemModel>
#include <QMenu>
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppPresence.h"
#include "../../Global.h"

class MainWindow;
class CRoster;

namespace Ui {
class CFrmMessage;
}

class CFrmMessage : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmMessage(QWidget *parent = 0);
    ~CFrmMessage();

    //注意：只在对话框初始化后调用一次,必须最先调用一次  
    int SetRoster(CRoster* pRoster);
    int AppendMessage(const QString &szMessage);
    int AppendMessageToList(const QString &szMessage, 
                            const QString &bareJid = CGlobal::Instance()->GetBareJid(),
                            const QString &name = CGlobal::Instance()->GetShowName(),
                            bool bRemote = false);

protected:
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent* );
    void closeEvent(QCloseEvent *e);

private slots:
    void on_pbBack_clicked();
    void on_pbSend_clicked();
    void on_tbMore_clicked();
    void on_pbVideo_clicked();
    void on_lbAvatar_clicked();//点击头像显示用户详细信息对话框  

    void ChangedPresence(QXmppPresence::AvailableStatusType status);

    void slotSendFileTriggered();//20140712文件发送  
#if WIN32
    void slotShotScreenTriggered();//20140712截屏  
#endif

private:
    Ui::CFrmMessage *ui;

    CRoster *m_pRoster;

    QStringList m_szMessages;
    QMenu m_MoreMenu;//功能按钮菜单  
};

#endif // FRMMESSAGE_H
