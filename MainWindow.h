#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "XmppClient.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppTransferManager.h"

class CFrmLogin;
class CFrmUserList;
class CDlgSendManage;

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

    CXmppClient* m_pClient;

    //在通知栏上显示消息  
    int ShowTrayIconMessage(const QString &szTitle, const QString &szMessage);
    
    //文件发送 
    void sendFile(const QString& jid,const QString& fileName,SendFileType type = MainWindow::DefaultType);
        
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

    void on_actionChange_Style_Sheet_S_triggered();
    void on_actionEnglish_E_triggered();
    void on_actionChinese_C_triggered();
    
    void on_actionNotifiation_show_main_windows_triggered();
    void on_actionNotifiation_status_online_triggered();
    void on_actionNotifiation_status_away_triggered();
    void on_actionNotifiation_status_dnd_triggered();
    void on_actionNotifiation_status_chat_triggered();
    void on_actionNotifiation_status_invisible_triggered();

    //编辑本地用户详细信息  
    void slotEditInformation();
    void slotClientVCardReceived();

    void onReceiveFile(QXmppTransferJob* job);//文件接收通知  

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionOptions_O_triggered();

private:
    int InitLoginedMenu();//初始化登录后的相关菜单  
    int AddStatusMenu(QMenu* pMenu);

private:
    Ui::MainWindow *ui;
    CFrmLogin *m_pLogin;
    CFrmUserList* m_pUserList;
    bool m_bLogin;

    QTranslator *m_pAppTranslator;
    QTranslator *m_pQtTranslator;

    QMenu m_TrayIconMenu;
    QSystemTrayIcon m_TrayIcon;
    CDlgSendManage* m_pSendManageDlg;//0712文件发送管理窗口  
};

#endif // MAINWINDOW_H
