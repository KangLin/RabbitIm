#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QSystemTrayIcon>
#ifndef MOBILE
    #include <QPropertyAnimation>
#endif
#include "Widgets/FrmMain/FrmMain.h"

#ifdef ANDROID
    #include "AndroidNotification.h"
#endif

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

private:
    int ReInitMenuOperator(); //初始化菜单  
    int InitLoginedMenu();    //初始化登录后的相关菜单  
    int InitOperatorMenu();   //初始始化操作菜单  

private:
    Ui::MainWindow *ui;
    QSharedPointer<CFrmLogin> m_Login;
    QSharedPointer<CFrmMain> m_TableMain;
    bool m_bLogin;//是否登录标志  

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
    QMenu m_MenuTranslate;
    QMap<QString, QAction*> m_ActionTranslator;
    QActionGroup m_ActionGroupTranslator;
    QSharedPointer<QTranslator> m_TranslatorQt;
    QSharedPointer<QTranslator> m_TranslatorApp;
    int InitMenuTranslate();  //增加翻译菜单  
    int ClearMenuTranslate();
    //加载翻译资源  
    int LoadTranslate(QString szLocale = QString());
private slots:
    //翻译菜单组点击触发  
    void slotActionGroupTranslateTriggered(QAction* pAct);

private:
    CDlgSendManage* m_pSendManageDlg;//0712文件发送管理窗口  

//程序更新功能  
#ifdef RABBITIM_USE_LIBCURL
public:
signals:
    void sigUpdateExec(int nError, const QString &szFile);
private slots:
    void slotUpdateExec(int nError, const QString &szFile);
#endif

//窗口靠边界时自动隐藏
#ifndef MOBILE
private:
    QPropertyAnimation m_Animation;
    bool m_bAnimationHide;//窗口靠边界时自动隐藏  
    int m_nWidth, m_nHeight;//窗口的宽和高  
    int m_nHideSize;//隐藏后的大小  
    int m_nBorderSize;//边界的大小  
    int AnimationWindows(const QRect &startRect, const QRect &endRect);
    
    int CheckShowWindows(QRect &endRect);
    QTimer m_timerAnimation;
private slots:
    void slotCheckHideWindows();
#endif

};

#endif // MAINWINDOW_H
