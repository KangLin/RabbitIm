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

    int GetUnreadCount();
    int ShowLastMessageDialog();

public slots:
    void slotMessageClean(const QString& szId);
private slots:
    void slotMessageUpdate(const QString& szId);

private:
    QList<QString> m_Unread;
    QList<QString> m_read;
};

#endif // MANAGERECENTMESSAGE_H
