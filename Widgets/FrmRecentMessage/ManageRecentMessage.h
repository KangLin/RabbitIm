#ifndef MANAGERECENTMESSAGE_H
#define MANAGERECENTMESSAGE_H
#include <QList>
#include <QObject>

class CManageRecentMessage : public QObject
{
    Q_OBJECT
public:
    CManageRecentMessage();
    ~CManageRecentMessage();
    int Init(const QString &szId);
    int Clean();

    /**
     * @brief 还有未读消息的好友数  
     *
     * @return int
     */
    int GetUnreadCount();
    /**
     * @brief 得到列表数  
     *
     * @return int
     */
    int GetAllCount();
    QList<QString> GetAllMembers();
    /**
     * @brief 返回列表容量最大值  
     *
     * @return int
     */
    int GetMaxCount();
    /**
     * @brief 显示最后接收未读消息对话框  
     *
     * @return int
     */
    int ShowLastMessageDialog();

    /**
     * @brief 删除指定的最近消息条目  
     *
     * @param szId：好友ID  
     * @return int
     */
    int Remove(const QString& szId);

public slots:
    /**
     * @brief 消息数清0时调用  
     *
     * @param szId：好友ID  
     */
    void slotMessageClean(const QString& szId);
private slots:
    void slotMessageUpdate(const QString& szId);

private:
    /**
     * @brief 从存储中加载信息  
     *
     * @param szLocaleJid  
     * @return int  
     */
    int LoadFromStorage(const QString &szId);
    /**
     * @brief 保存信息到存储  
     *
     * @return int  
     */
    int SaveToStorage();
private:
    QList<QString> m_Unread;//未读消息列表  
    QList<QString> m_read;  //已读的消息列表  
};

#endif // MANAGERECENTMESSAGE_H
