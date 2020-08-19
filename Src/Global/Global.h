#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>
#include <QSharedPointer>
#include <QDateTime>
#include "Manage/Manager.h"
#include "common/Tool.h"
#include <QDebug>
#include "Log.h"

class RABBITIM_SHARED_LIBRARY MainWindow;
class RABBITIM_SHARED_LIBRARY CLog;

/** 
 * @mainpage
 * 
 *
 */

/**
  * @defgroup RABBITIM_GLOBAL 全局变更类模块  
  * @brief 全局变量模块  
  */

/**
 * @brief The CGlobal class
 * @ingroup RABBITIM_GLOBAL
 */
class RABBITIM_SHARED_LIBRARY CGlobal : public QObject
{
    Q_OBJECT
private:
    explicit CGlobal(QObject *parent = nullptr);
    virtual ~CGlobal();

public:
    static CGlobal* Instance();

public:
    /// 得到主窗口  
    MainWindow* GetMainWindow();
    int SetMainWindow(MainWindow* pWnd);
    CManager* GetManager();
    int SetManager(CManager* pManager);
private:
    MainWindow* m_pMainWindow;
    CManager* m_pManager;

public:
    // 只保存用户登录时的状态,在用户登录对话框中设置  
    CUserInfo::USER_INFO_STATUS GetStatus();///< 得到本地用户状态  
    int SetStatus(CUserInfo::USER_INFO_STATUS status);
private:
    CUserInfo::USER_INFO_STATUS m_LocalStatus;///< 本地用户的状态  

public:
    /// 好友状态文本表示  
    QString GetRosterStatusText(CUserInfo::USER_INFO_STATUS status);
    /// 好友状态图标资源字符串  
    QString GetRosterStatusIcon(CUserInfo::USER_INFO_STATUS status);
    /// 好友状态颜色表示  
    QColor GetRosterStatusColor(CUserInfo::USER_INFO_STATUS status);

public:
    /// 设置本地用户的显示颜色  
    int SetUserColor(const QColor &color);
    QColor GetUserColor();
    /// 设置好友的显示颜色  
    int SetRosterColor(const QColor &color);
    QColor GetRosterColor();
    /// 设置消息字体颜色  
    int SetUserMessageColor(const QColor &color);
    QColor GetUserMessageColor();
    /// 设置好友消息字体颜色  
    int SetRosterMessageColor(const QColor &color);
    QColor GetRosterMessageColor();
    /// 设置未读消息记数颜色  
    int SetUnreadMessageCountColor(const QColor &color);
    QColor GetUnreadMessageCountColor();
private:
    QColor m_UserColor;   ///< 本地用户颜色  
    QColor m_RosterColor; ///< 好友颜色  
    QColor m_UserMessageColor;
    QColor m_RosterMessageColor;
    QColor m_UnreadMessageCountColor;///< 未读消息记数颜色  
    QColor GetColorFormConf(const QString &Key, const QColor &def);
    int SetColorToConf(const QString &Key, const QColor &color);

public:
    /**
     * @brief 主窗口关闭类型  
     */
    enum E_CLOSE_TYPE
    {
        E_CLOSE_TYPE_NO,
        E_CLOSE_TYPE_CLOSE_PROGRAME, ///< 退出程序  
        E_CLOSE_TYPE_LOGOUT          ///< 登出  
    };
    E_CLOSE_TYPE GetCloseType();
    int SetCloseType(E_CLOSE_TYPE type);

private:
    E_CLOSE_TYPE m_CloseType;

public:
    /**
     * @brief 消息对话框发送消息快捷键  
     */
    enum E_MESSAGE_SEND_TYPE
    {
        E_MESSAGE_SEND_TYPE_ENTER,     ///< enter 发送消息  
        E_MESSAGE_SEND_TYPE_CTRL_ENTER ///< ctrl+enter 发送消息  
    };
    int SetMessageSendType(E_MESSAGE_SEND_TYPE type);
    E_MESSAGE_SEND_TYPE GetMessageSendType();
private:
    E_MESSAGE_SEND_TYPE m_MessageSendType;

public:
    QString GetXmppServer();
    int SetXmppServer(QString host);
    qint16 GetXmppServerPort();
    int SetXmppServerPort(qint16 port);
    QString GetXmppDomain();
    int SetXmppDomain(QString server);
    QString GetStunServer();
    int SetStunServer(QString server);
    qint16 GetStunServerPort();
    int SetStunServerPort(qint16 port);
    QString GetTurnServer();
    int SetTurnServer(QString server);
    qint16 GetTurnServerPort();
    int SetTurnServerPort(qint16 port);
    QString GetTurnServerUser();
    int SetTurnServerUser(QString user);
    QString GetTurnServerPassword();
    int SetTurnServerPassword(QString password);
private:
    QString m_szXmppServer; //Xmpp服务器IP地址   
    int m_szXmppServerPort;
    QString m_szXmppDomain; //Xmpp服务器地址  
    QString m_szTurnServer; //Turn服务器地址  
    int m_nTurnServerPort;
    QString m_szTurnUser;
    QString m_szTurnPassword;
    QString m_szStunServer; //Stun服务器地址  
    int m_nStunServerPort;

    //配置选项  
public:
    //设置自动登录  
    int SetAutoLogin(bool bAuto);
    bool GetAutoLogin();
    int SetAutoLoginDelayTime(int nTime); ///< 单位：S
    int GetAutoLoginDelayTime();
    //设置是否在通知栏弹出消息  
    int SetNotifiationBarShowMessage(bool bShowMessage);
    bool IsNotifiationBarShowMessage();
    //设置通知栏弹出消息显示的时间  
    int SetNotifiationBarShowMessageDelay(int nMs); ///< 单位：ms  
    int GetNotifiationBarShowMessageDelay();        ///< 单位：ms  
    //设置通知栏图标是否闪烁  
    int SetNotifiationFlashs(bool bFlashs);
    bool IsNotifiationFlashs();
    //设置闪烁间隔时间  
    int SetNotifiationFlashInterval(int nMs); ///< 单位：ms  
    int GetNotifiationFlashInterval();        ///< 单位：ms  
    //是否提示接收消息提示音  
    int SetMessageSound(bool bSound);
    bool GetMessageSound();
    //主窗口在边界自动隐藏的延时  
    int SetAnimationHideMainWindow(int nMs); ///< 单位:ms  
    int GetAnimationHideMainWindow();        ///< 单位:ms  
    int SetAnimationDuration(int nMs);       ///< 单位:ms  
    int GetAnimationDuration();              ///< 单位:ms  
    bool IsAnimationHideMainWindow();        ///< 是否自动隐藏主窗口  
    int SetIsAnimationHideMainWindow(bool bHide);

private:
    bool m_AutoLogin;               ///< 自动登录  
    int m_nAutoLoginDelayTime;
    bool m_bNotifiationBarShowMessage;
    int m_nShowMessageDelay;
    bool m_bNotifiationBarFlashs;
    int m_nFlashInterval;
    bool m_bMessageSound;           ///< 是否提示接收消息提示音  
    int m_nAnimationHideMainWindow; ///< 单位:ms  
    int m_nAnimationDuration;       ///< 动画时长.单位:ms  
    bool m_bAnimationHideMainWindows;

public:
    /** 
     * 配置好友显示名称  
     */
    enum E_ROSTER_SHOW_TYPE
    {
        E_ROSTER_SHOW_JID,
        E_ROSTER_SHOW_NAME,
        E_ROSTER_SHOW_NICK
    };
    E_ROSTER_SHOW_TYPE GetRosterShowType();
    int SetRosterShowType(E_ROSTER_SHOW_TYPE type);
    
    enum E_SCREEN_SHOT_TO_TYPE //截屏后文件处理  
    {
        E_TO_CLIPBOARD,
        E_TO_SEND,
        E_TO_SAVE,
        E_TO_QRCODE
    };
    
    E_SCREEN_SHOT_TO_TYPE GetScreenShotToType();
    int SetScreenShotToType(E_SCREEN_SHOT_TO_TYPE type);
    bool IsHideMessageBox();//截屏时是否隐藏消息对话框窗口  
    int SetHideMessageBox(bool bHide);
    
private:
    E_ROSTER_SHOW_TYPE m_RosterShowType;
    E_SCREEN_SHOT_TO_TYPE m_ScreenShotToType;
    bool m_bHideMessageBox;

public:
    int GetVideoCaptureDevice();
    int SetVideoCaptureDevice(int nIndex = 0);
Q_SIGNALS:
    void sigVideoCaptureDevice(int nIndex);
private:
    int m_nVideoCaptureDevice;
public:
    int GetAudioInputDevice();
    int SetAudioInputDevice(int nIndex);
    int GetAudioOutputDevice();
    int SetAudioOutputDevice(int nIndex);
private:
    int m_nAudioInputDevice;
    int m_nAudioOutputDevice;

public:
    //显示本地视频  
    bool GetIsShowLocaleVideo();
    int SetIsShowLocaleVideo(bool bShow = true);
    bool GetIsMonitor();
    int SetMonitor(bool bMonitor);
private:
    bool m_bShowLocaleVideo;
    bool m_bMonitor;

public:
    QString GetStyle();
    QString GetStyleMenu();
    int SetStyleMenu(QString szMenu, QString szFile);
private:
    QString m_szStyleFile;
    QString m_szStyleMenu;

public:
    /** 
     * 得到表情图片文件  
     */
    QString GetFileEmoji();
    int SetFileEmoji(const QString &szFile);
Q_SIGNALS:
    void sigEmojiChanged();
private:
    QString m_szEmoji;
    
public:
    int GetEmojiFontPointSize();
    int SetEmojiFontPointSize(int x);
private:
    int m_EmojiFontPointSize;
    
signals:

public slots:
};

#define GETMANAGER CGlobal::Instance()->GetManager()
#define GET_MAINWINDOW CGlobal::Instance()->GetMainWindow()
#define GET_CLIENT GETMANAGER->GetClient()
#define GLOBAL_USER GETMANAGER->GetManageUser()
#define USER_INFO_LOCALE GLOBAL_USER->GetUserInfoLocale()
#define MANAGE_MESSAGE_DIALOG GETMANAGER->GetManageMessageDialog()

#define RABBITIM_ICON_SIZE 24 //头像的大小  
#define RABBITIM_AVATAR_SIZE 64 //头像的大小  

#ifdef DEBUG
    #define RABBITIM_ASSERT(x, ret) Q_ASSERT(x)
#else
    #define RABBITIM_ASSERT(x, ret) \
        if(!(x))                    \
            return (ret);
#endif //DEBUG

#endif // GLOBAL_H
