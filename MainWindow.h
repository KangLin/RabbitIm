#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "XmppClient.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppTransferManager.h"
#include "Widgets/FrmMain/FrmMain.h"

class CFrmLogin;
class CFrmUserList;
class CDlgSendManage;
class CRoster;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* 发送文件类型
     * DefaultType:默认类型
     * ImageType:图片*/
    enum SendFileType{
        DefaultType,
        ImageType
    };
    
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //得到好友 CRoster 对象  
    CRoster* GetRoster(QString szJid);

    //在通知栏上显示消息  
    int ShowTrayIconMessage(const QString &szTitle, const QString &szMessage);
    
    //文件发送  
    void sendFile(const QString& jid,const QString& fileName,SendFileType type = MainWindow::DefaultType);

signals:
    //初始化菜单  
    void sigInitLoginedMenu(QMenu* m);
    //删除菜单  
    void sigRemoveMenu(QMenu* m);
    void sigRefresh();//选项窗体更新后，刷新内容  

protected slots:
    void About();
    void clientConnected();
    void clientDisconnected();
    void clientError(QXmppClient::Error e);
    void clientIqReceived(const QXmppIq &iq);
    void stateChanged(QXmppClient::State state);

    void slotTrayIconActive(QSystemTrayIcon::ActivationReason e);//通知栏图标槽  
    void slotMessageClicked();
    void slotTrayIconMenuUpdate();
    void slotTrayTimeOut();
    void slotTrayTimerStart();
    void slotTrayTimerStop();

    void on_actionNotifiation_show_main_windows_triggered();
    void slotActionGroupStatusTriggered(QAction* act);//状态菜单触发  

    void on_actionChange_Style_Sheet_S_triggered();
    void on_actionEnglish_E_triggered();
    void on_actionChinese_C_triggered();

    //编辑本地用户详细信息  
    void slotEditInformation();
    void slotClientVCardReceived();

    void onReceiveFile(QXmppTransferJob* job);//文件接收通知  

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionOptions_O_triggered();

private:
    int InitMenu();       //初始化菜单  
    int InitLoginedMenu();//初始化登录后的相关菜单  
    int InitOperatorMenu();//初始始化操作菜单  
    int AddStatusMenu(QMenu* pMenu);//增加状态菜单  

private:
    Ui::MainWindow *ui;
    CFrmLogin *m_pLogin;
    CFrmMain *m_pTableMain;
    bool m_bLogin;

    QTranslator *m_pAppTranslator;
    QTranslator *m_pQtTranslator;

    QMenu m_TrayIconMenu;
    QSystemTrayIcon m_TrayIcon;
    bool m_bTrayShow;
    QTimer m_TrayTimer;

    QMenu m_MenuStatus;
    QAction* m_ActionStatusOnline;
    QAction* m_ActionStatusAway;
    QAction* m_ActionStatusDnd;
    QAction* m_ActionStatusChat;
    QAction* m_ActionStatusInvisible;
    QActionGroup m_ActionGroupStatus;
    
    CDlgSendManage* m_pSendManageDlg;//0712文件发送管理窗口  
};

#endif // MAINWINDOW_H
