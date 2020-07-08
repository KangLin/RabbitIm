#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QSystemTrayIcon>
#ifndef MOBILE
    #include "Widgets/MainAnimation.h"
#endif
#include "Widgets/FrmMain/FrmMain.h"

#ifdef ANDROID
    #include "android/jni/AndroidNotification.h"
#endif
#include "Global/Global.h"

class CFrmLogin;
class CFrmUserList;
class CDlgSendManage;

namespace Ui {
class MainWindow;
}

class RABBITIM_SHARED_LIBRARY MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// 在通知栏上显示消息  
    int ShowTrayIconMessage(const QString &szTitle, const QString &szMessage);

    /// 组合头像图片  
    static int ComposeAvatarStatus(QSharedPointer<CUserInfo> info,
                                   QPixmap &outPixmap);

signals:
    ///初始化菜单  
    void sigMenuInitOperator(QMenu* m);
    ///删除菜单  
    void sigMenuRemoveOperator(QMenu* m);
    /// 选项窗体更新后触发。用于刷新与选项相关的内容  
    void sigRefresh();

protected slots:
    void slotAbout();
    void slotClientConnected();
    void slotClientDisconnected();
    ///更新本地用户信息  
    void slotUpdateLocaleUserInfo();

    //通知栏  
    void slotTrayIconActive(QSystemTrayIcon::ActivationReason e);///<通知栏图标槽  
    void slotMessageClicked();
    void slotTrayIconMenuUpdate();
    void slotTrayTimeOut();
    void slotTrayTimerStart();
    void slotTrayTimerStop();

    void slotMessageClean(const QString& szId);

    void on_actionNotifiation_show_main_windows_triggered();

    //登录用户登出  
    void slotLogout();
public slots:
    //编辑登录用户详细信息  
    void slotEditInformation();

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *e);
    virtual void changeEvent(QEvent* e);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void moveEvent(QMoveEvent* event);
    //事件监听器  
    virtual bool eventFilter(QObject *target, QEvent *event);

private slots:
    void on_actionOptions_O_triggered();
    //扫描二维码  
    void on_actionScan_qrencode_S_triggered();

private:
    int ReInitMenuOperator(); //初始化菜单  
    int InitLoginedMenu();    //初始化登录后的相关菜单  
    int InitOperatorMenu();   //初始始化操作菜单  

private:
    Ui::MainWindow *ui;
    QSharedPointer<CFrmLogin> m_Login;
    QSharedPointer<CFrmMain> m_TableMain;
    bool m_bLogin;//是否登录标志  
    int m_nLoginFlag; //0:未定义，1：关闭按钮

    //系统栏提示  
    QMenu m_TrayIconMenu;
    QSystemTrayIcon m_TrayIcon;
    bool m_bTrayShow;
    QTimer m_TrayTimer;
#ifdef ANDROID
public:
    CAndroidNotification m_AndroidNotify;
#endif

private:
    //样式菜单  
    QMenu m_MenuStyle;
    QActionGroup m_ActionGroupStyle;
    QMap<QString, QAction*> m_ActionStyles;
    int InitMenuStyles();
    int ClearMenuStyles();
    int OpenCustomStyleMenu();
    int LoadStyle();
private slots:
    void slotActionGroupStyleTriggered(QAction* act);//改变样式菜单触发  

private:
    //状态菜单  
    QMenu m_MenuStatus;
    QActionGroup m_ActionGroupStatus;
    QMap<CUserInfo::USER_INFO_STATUS, QAction*> m_ActionStatus;
    int InitMenuStatus();     //增加状态菜单  
    int ClearMenuStatus();
private slots:
    void slotActionGroupStatusTriggered(QAction* act);//状态菜单组点击触发  

private:
    //翻译菜单  
    struct _MENU{
        QString icon;
        QString text;
    };
    QMenu m_MenuTranslate;
    QMap<QString, QAction*> m_ActionTranslator;
    QActionGroup m_ActionGroupTranslator;
    QSharedPointer<QTranslator> m_TranslatorQt;
    QSharedPointer<QTranslator> m_TranslatorApp;
    int InitMenuTranslate();  //增加翻译菜单  
    int ClearMenuTranslate();
    //加载翻译资源  
    int LoadTranslate(QString szLocale = QString());
    int ClearTranslate(); //卸载翻译资源  
private slots:
    //翻译菜单组点击触发  
    void slotActionGroupTranslateTriggered(QAction* pAct);

private:
    CDlgSendManage* m_pSendManageDlg;//文件发送管理窗口  

//窗口靠边界时自动隐藏
#ifndef MOBILE
private:
    QSharedPointer<QPropertyAnimation> m_Animation;
    CMainAnimation m_MainAnimation;
    bool m_bAnimationHide;//窗口靠边界时自动隐藏   
    int m_nHideSize;//隐藏后的提示框大小  
    int m_nBorderSize;//边界区域的大小  
    int AnimationWindows(const QRect &startRect, const QRect &endRect);
    QTimer m_timerAnimation;

private slots:
    void slotCheckHideWindows();
    void slotCheckShowWindows();
    void slotAnimationFinished();
#endif
};

#endif // MAINWINDOW_H
