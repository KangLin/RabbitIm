#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppClient.h"
#include "XmppClient.h"

class CRoster;
class MainWindow;
class CGlobal : public QObject
{
    Q_OBJECT
private:
    explicit CGlobal(QObject *parent = 0);
    virtual ~CGlobal();

public:
    static CGlobal* Instance();

    //日志  
    //参数：  
    //    pszFile:打印日志处文件名  
    //    nLine:打印日志处行号  
    //    nLevel:打印日志错误级别  
    //....pszModelName:打印日志的模块范围  
    int Log(const char *pszFile, int nLine, int nLevel, const char* pszModelName, const char *pFormatString, ...);

public:
    //得到主窗口  
    MainWindow* GetMainWindow();
    int SetMainWindow(MainWindow* pWnd);
    CXmppClient* GetXmppClient();
private:
    MainWindow* m_pMainWindow;
    CXmppClient m_XmppClient;

public:
    //用户数据存放目录  
    QString GetDirUserData(const QString bareJid = QString());
    //得到用户头像目录  
    QString GetDirUserAvatar();
    //得到指定用户的头像文件  
    QString GetFileUserAvatar(QString bareJid);
    //应用程序目录  
    QString GetDirApplication();
    //应用程序配置目录  
    QString GetDirApplicationConfigure();
    //应用程序配置文件  
    QString GetApplicationConfigureFile();

    //得到本地用户住息  
    QString GetShowName();//根据配置显示用户名称  
    QString GetName();
    QString GetJid();
    QString GetBareJid();
    QString GetDomain();
    QString GetResource();
    CRoster* GetRoster(); //本地用户详细信息  
    QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    int SetStatus(QXmppPresence::AvailableStatusType status);
private:
    CRoster *m_pRoster;     //存储本地用户的详细信息  
    QXmppPresence::AvailableStatusType m_LocalStatus;//本地用户的状态  

public:
    //好友状态文本表示  
    QString GetRosterStatusText(QXmppPresence::AvailableStatusType status);
    //好友状态图标资源字符串  
    QString GetRosterStatusIcon(QXmppPresence::AvailableStatusType status);
    //好友状态颜色表示  
    QColor GetRosterStatusColor(QXmppPresence::AvailableStatusType status);

public:
    //设置本地用户的显示颜色  
    int SetUserColor(const QColor &color);
    QColor GetUserColor();
    //设置好友的显示颜色  
    int SetRosterColor(const QColor &color);
    QColor GetRosterColor();
    //设置消息字体颜色  
    int SetUserMessageColor(const QColor &color);
    QColor GetUserMessageColor();
    //设置好友消息字体颜色  
    int SetRosterMessageColor(const QColor &color);
    QColor GetRosterMessageColor();
    //设置未读消息记数颜色  
    int SetUnreadMessageCountColor(const QColor &color);
    QColor GetUnreadMessageCountColor();
private:
    QColor m_UserColor;   //本地用户颜色  
    QColor m_RosterColor; //好友颜色  
    QColor m_UserMessageColor;
    QColor m_RosterMessageColor;
    QColor m_UnreadMessageCountColor;//未读消息记数颜色  
    QColor GetColorFormConf(const QString &Key, const QColor &def);
    int SetColorToConf(const QString &Key, const QColor &color);

public:
    enum E_MESSAGE_SEND_TYPE
    {
        E_MESSAGE_SEND_TYPE_ENTER,
        E_MESSAGE_SEND_TYPE_CTRL_ENTER
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
    int SetAutoLoginDelayTime(int nTime);
    int GetAutoLoginDelayTime();
    //设置是否在通知栏弹出消息  
    int SetNotifiationBarShowMessage(bool bShowMessage);
    bool IsNotifiationBarShowMessage();
    //设置通知栏弹出消息显示的时间  
    int SetNotifiationBarShowMessageDelay(int nMs);//单位：ms  
    int GetNotifiationBarShowMessageDelay();//单位：ms  
    //设置通知栏图标是否闪烁  
    int SetNotifiationFlashs(bool bFlashs);
    bool IsNotifiationFlashs();
    //设置闪烁间隔时间  
    int SetNotifiationFlashInterval(int nMs);//单位：ms  
    int GetNotifiationFlashInterval();//单位：ms  


private:
    bool m_AutoLogin;//自动登录  
    int m_nAutoLoginDelayTime;
    bool m_bNotifiationBarShowMessage;
    int m_nShowMessageDelay;
    bool m_bNotifiationBarFlashs;
    int m_nFlashInterval;

public:
    //配置好友显示名称  
    enum E_ROSTER_SHOW_TYPE
    {
        E_ROSTER_SHOW_JID,
        E_ROSTER_SHOW_NAME,
        E_ROSTER_SHOW_NICK
    };
    E_ROSTER_SHOW_TYPE GetRosterShowType();
    int SetRosterShowType(E_ROSTER_SHOW_TYPE type);
    
    enum E_SCREEN_SHOT_TO_TYPE //20140802 截屏后文件处理  
    {
        E_TO_CLIPBOARD,
        E_TO_SEND,
        E_TO_SAVE
    };
    
    E_SCREEN_SHOT_TO_TYPE GetScreenShotToType();
    int SetScreenShotToType(E_SCREEN_SHOT_TO_TYPE type);
    
private:
    E_ROSTER_SHOW_TYPE m_RosterShowType;
    E_SCREEN_SHOT_TO_TYPE m_ScreenShotToType;

signals:

public slots:
};

#define LM_DEBUG 0
#define LM_INFO 1
#define LM_WARNING 2
#define LM_ERROR 3

#ifdef DEBUG
#define LOG_ERROR(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_DEBUG, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_INFO(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(...)
#define LOG_WARNING(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...)
#define LOG_MODEL_INFO(model, fmt, ...) CGlobal::Instance()->Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)
#endif

#endif // GLOBAL_H
