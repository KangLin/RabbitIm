#ifndef MANAGEGROUPCHAT_H
#define MANAGEGROUPCHAT_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include "GroupChat.h"

class CManageGroupChat : public QObject
{
    Q_OBJECT
public:
    explicit CManageGroupChat(QObject *parent = 0);
    virtual ~CManageGroupChat();

    /**
     * @brief 登录后初始化  
     *
     * @param szId
     * @return int
     */
    virtual int Init(const QString& szId);
    virtual int Clean();

    virtual int Create(
                   const QString &szName,
                   const QString &szSubject,
                   const QString &szPassword = QString(),
                   const QString &szDescription = QString(),
                   bool bProtracted = false,
                   bool bPrivated = false,
                   const QString &szNick = QString()
            ) = 0;
    virtual int Join(const QString &szId, const QString &szNick = QString()) = 0;
    virtual QSharedPointer<CGroupChat> Get(const QString &szId) = 0;

signals:
    void sigJoined(const QString& szId);
    void sigLeave(const QString& szId);
    void sigUpdateMessage(const QString &szId);

    void sigMessageClean(const QString &szId);
public slots:
    int slotLeave(const QString& szId);

protected:
    /**
     * @brief 从存储中加载信息  
     *
     * @param szLocaleJid  
     * @return int  
     */
    virtual int LoadFromStorage(const QString &szId);
    /**
     * @brief 保存信息到存储  
     *
     * @return int  
     */
    virtual int SaveToStorage();

protected:
    QMap<QString, QSharedPointer<CGroupChat> > m_GroupChat;

};

#endif // MANAGEGROUPCHAT_H
