#ifndef ROSTER_H
#define ROSTER_H

#include <QObject>
#include <QStandardItemModel>
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppPresence.h"
#include <QSet>
#include "../FrmMessage/FrmMessage.h"
#include "../FrmVideo/FrmVideo.h"

class MainWindow;

class CRoster : public QObject
{
    Q_OBJECT
public:
    //参数 parent 必须为 MainWindow
    explicit CRoster(QObject *parent = 0);
    //parent 必须为 MainWindow
    explicit CRoster(QString jid, QSet<QString> groups, MainWindow* parent = 0);
    ~CRoster();

    QString Name();
    QString Domain();
    QString Resouce();
    QString BareJid();
    QString Jid();
    int SetJid(QString jid);

    QSet<QString> Groups();
    int SetGroups(const QSet<QString> &groups);

    QList<QStandardItem *> GetItem(); //得到条目对象

    QXmppPresence::AvailableStatusType GetStatus();
    //状态改变
    int ChangedPresence(QXmppPresence::AvailableStatusType status);
    //显示消息对话框
    int ShowMessageDialog();
    //增加消息
    int AppendMessage(const QString &szMessage);
    int CleanNewMessageNumber();

    CFrmVideo m_Video;

private:
    int Init(MainWindow *parent = 0);

signals:
    void ReciveMessage(CRoster* pRoster);
    //好友状态改变时触发
    void sigChangedPresence(QXmppPresence::AvailableStatusType status);

public slots:

private:
    QString m_szJid;
    QXmppPresence::AvailableStatusType m_Status;

    MainWindow* m_pMainWindow;
    std::list<QStandardItem*> m_lstUserListItem; //这个要交给控件释放
    QSet<QString> m_Groups;
    //TODO:改成动态产生
    CFrmMessage m_Message;
    int m_nNewMessageNumber;//新消息数

};

#endif // ROSTER_H
