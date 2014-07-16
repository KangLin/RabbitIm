#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppClient.h"

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

    //用户数据存放目录 
    QString GetDirUserData(const QString bareJid = QString());
    //应用程序目录 
    QString GetDirApplication();
    //应用程序配置目录  
    QString GetDirApplicationConfigure();
    //应用程序配置文件  
    QString GetApplicationConfigureFile();

    //得到本地用户住息  
    int SetJid(QString jid);
    QString GetName();
    QString GetBareJid();
    QString GetDomain();
    QString GetResource();
    QColor GetUserColor();
    QColor GetRosterColor();
    int SetUserColor(const QColor &color);
    int SetRosterColor(const QColor &color);
private:
    QString m_szLocalJid;
    QColor m_UserColor;   //本地用户颜色  
    QColor m_RosterColor; //好友颜色  

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
    
public:
    //好友状态文本表示 
    QString GetStatusText(QXmppPresence::AvailableStatusType status);
    //好友状态颜色表示 
    QColor GetStatusColor(QXmppPresence::AvailableStatusType status);
    
    //配置选项 
public:
    bool IsNotifiationBarShowMessage();
    int SetNotifiationBarShowMessage(bool bShowMessage);
    bool IsNotifiationFlashs();
    int SetNotifiationFlashs(bool bFlashs);
private:
    bool m_bNotifiationBarShowMessage;
    bool m_bNotifiationBarFlashs;

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
