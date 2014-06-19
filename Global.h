#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QColor>
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppClient.h"

class CGlobal : public QObject
{
    Q_OBJECT
public:
    explicit CGlobal(QObject *parent = 0);
    virtual ~CGlobal();

    //日志
    //参数： 
    //    pszFile:打印日志处文件名 
    //    nLine:打印日志处行号 
    //    nLevel:打印日志错误级别 
    //....pszModelName:打印日志的模块范围 
    int Log(const char *pszFile, int nLine, int nLevel, const char* pszModelName, const char *pFormatString, ...);
    
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

    QString GetXmppServerHost();
    int SetXmppServerHost(QString &host);
    int SetXmppServerPort(qint16 port);
    qint16 GetXmppServerPort();
    QString GetXmppServer();
    int SetXmppServer(QString server);
    QString GetStunServer();
    int SetStunServer(QString &server);
    qint16 GetStunServerPort();
    int SetStunServerPort(qint16 &port);
    QString GetTurnServer();
    int SetTurnServer(QString &server);
    qint16 GetTurnServerPort();
    int SetTurnServerPort(qint16 &port);
    QString GetTurnServerUser();
    int SetTurnServerUser(QString &user);
    QString GetTurnServerPassword();
    int SetTurnServerPassword(QString &password);

    //好友状态文本表示
    QString GetStatusText(QXmppPresence::AvailableStatusType status);
    //好友状态颜色表示
    QColor GetStatusColor(QXmppPresence::AvailableStatusType status);

    //用户数据存放目录
    QString GetDirUserData(const QString bareJid = QString());
    //应用程序目录
    QString GetDirApplication();
    //应用程序配置目录
    QString GetDirApplicationConfigure();

    enum __STYLE_SHEET_TYPE
    {
        STYPLE_SHEET_FORM_BACKGROUND  //窗体背景样式
    };
    //得到默认样式
    QString GetStyleSheet(const __STYLE_SHEET_TYPE t = CGlobal::STYPLE_SHEET_FORM_BACKGROUND);
    //设置窗口样式,返回原来窗口样式
    QString SetStyleSheet(const QString szStyleSheet, const __STYLE_SHEET_TYPE t =CGlobal::STYPLE_SHEET_FORM_BACKGROUND);
    QString SetStyleSheet(QWidget *pWidget, const __STYLE_SHEET_TYPE t = STYPLE_SHEET_FORM_BACKGROUND);
    
signals:

public slots:

private:
    QString m_szLocalJid;
    QColor m_UserColor;   //本地用户颜色
    QColor m_RosterColor; //好友颜色

    QString m_szXmppServerHost; //Xmpp服务器IP地址
    int m_szXmppServerPort;
    QString m_szXmppServer; //Xmpp服务器地址
    QString m_szTurnServer; //Turn服务器地址
    int m_nTurnServerPort;
    QString m_szTurnUser;
    QString m_szTurnPassword;
    QString m_szStunServer; //Stun服务器地址
    int m_nStunServerPort;
    
    QString m_szFormBackGroundStyleSheet; //样式
};

extern CGlobal g_Global;

#define LM_DEBUG 0
#define LM_INFO 1
#define LM_WARNING 2
#define LM_ERROR 3

#ifdef DEBUG
#define LOG_ERROR(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_DEBUG, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_DEBUG, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_INFO(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_ERROR, "", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(...)
#define LOG_WARNING(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_WARNING, "", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_INFO, "", fmt, ##__VA_ARGS__)

#define LOG_MODEL_ERROR(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_ERROR, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_WARNING(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_WARNING, model, fmt, ##__VA_ARGS__)
#define LOG_MODEL_DEBUG(model, fmt, ...)
#define LOG_MODEL_INFO(model, fmt, ...) g_Global.Log(__FILE__, __LINE__, LM_INFO, model, fmt, ##__VA_ARGS__)
#endif

#endif // GLOBAL_H
