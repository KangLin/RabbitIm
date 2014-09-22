#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QSystemTrayIcon>
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
     * ImageType:图片  */
    enum SendFileType{
        DefaultType,
        ImageType
    };
    
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //得到好友 CRoster 对象指针  
    CRoster* GetRoster(QString szJid);
    //得到聊天室 CFrmGroupChat 对象指针  
    CFrmGroupChat* GetGroupChat(const QString& jid);

    //在通知栏上显示消息  
    int ShowTrayIconMessage(const QString &szTitle, const QString &szMessage);
    
    //文件发送  
    void sendFile(const QString& jid,const QString& fileName,SendFileType type = MainWindow::DefaultType);

signals:
    //初始化菜单  
    void sigMenuInitOperator(QMenu* m);
    //删除菜单  
    void sigMenuRemoveOperator(QMenu* m);
    void sigRefresh();//选项窗体更新后，刷新内容  

protected slots:
    void About();
    void slotClientConnected();
    void slotClientDisconnected();
    //更新本地用户信息  
    void slotUpdateLocaleUserInfo();

    //通知栏  
    void slotTrayIconActive(QSystemTrayIcon::ActivationReason e);//通知栏图标槽  
    void slotMessageClicked();
    void slotTrayIconMenuUpdate();
    void slotTrayTimeOut();
    void slotTrayTimerStart();
    void slotTrayTimerStop();

    void on_actionNotifiation_show_main_windows_triggered();

    void slotActionGroupStatusTriggered(QAction* act);//状态菜单组点击触发  
    void slotActionGroupTranslateTriggered(QAction* pAct);//翻译菜单组点击触发 

    void on_actionChange_Style_Sheet_S_triggered();//改变样式菜单触发  

    //编辑登录用户详细信息  
    void slotEditInformation();
    //登录用户登出  
    void slotLogout();
    void onReceiveFile(QXmppTransferJob* job);//文件接收通知  

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *e);
    virtual void changeEvent(QEvent* e);

private slots:
    void on_actionOptions_O_triggered();

private:
    int ReInitMenuOperator(); //初始化菜单  
    int InitLoginedMenu();    //初始化登录后的相关菜单  
    int InitOperatorMenu();   //初始始化操作菜单  
    int InitMenuStatus();     //增加状态菜单  
    int ClearMenuStatus();
    int InitMenuTranslate();  //增加翻译菜单  
    int ClearMenuTranslate();
    int LoadTranslate(QString szLocale = QString());

private:
    Ui::MainWindow *ui;
    QSharedPointer<CFrmLogin> m_Login;
    QSharedPointer<CFrmMain> m_TableMain;
    bool m_bLogin;

    QMenu m_TrayIconMenu;
    QSystemTrayIcon m_TrayIcon;
    bool m_bTrayShow;
    QTimer m_TrayTimer;

    //状态菜单  
    QMenu m_MenuStatus;
    QActionGroup m_ActionGroupStatus;
    QMap<CUserInfo::USER_INFO_STATUS, QAction*> m_ActionStatus;

    //翻译菜单  
    QMenu m_MenuTranslate;
    QMap<QString, QAction*> m_ActionTranslator;
    QActionGroup m_ActionGroupTranslator;
    QTranslator *m_pTranslatorQt;
    QTranslator *m_pTranslatorApp;

    CDlgSendManage* m_pSendManageDlg;//0712文件发送管理窗口  
};

#endif // MAINWINDOW_H
