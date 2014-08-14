#ifndef ROSTER_H
#define ROSTER_H

#include <QSet>
#include <QObject>
#include <QStandardItemModel>
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppRosterIq.h"
#include "qxmpp/QXmppVCardIq.h"
#include "../FrmMessage/FrmMessage.h"

class MainWindow;

class CRoster : public QObject
{
    Q_OBJECT
public:
    //参数 parent 必须为 MainWindow  
    explicit CRoster(QObject *parent = NULL);
    //parent 必须为 MainWindow  
    explicit CRoster(QXmppRosterIq::Item item, QObject* parent = NULL);
    ~CRoster();

    QString ShowName();//用于显示好友的名字,这个函数可以根据配置显示友好名称  
    QString Name();//如果用于显示好友名字，请用ShowName  
    QString Jid();
    QString Nick();
    QString Domain();
    QString Resouce();
    QString BareJid();
    //int SetJid(QString jid);//由SetVCard或UpdaeItems设置  
    QDate Birthday();
    QString Email();
    QString Description();
    QImage Photo();

    int SetVCard(const QXmppVCardIq &vCard);

    QSet<QString> Groups();

    QList<QStandardItem *> GetItem(); //得到条目对象  
    int UpdateItems(QXmppRosterIq::Item item);//更新与此好友相关条目对象内容  

    QXmppPresence::AvailableStatusType GetStatus();
    QXmppRosterIq::Item::SubscriptionType GetSubScriptionType();

    //状态改变  
    int ChangedPresence(const QString &jid, QXmppPresence::AvailableStatusType status);
    //显示消息对话框  
    int ShowMessageDialog();
    //增加消息  
    int AppendMessage(const QString &szMessage);
    int CleanNewMessageNumber();

public slots:
    //刷新好友  
    void slotRefresh();

private:
    int Init();
    QString GetSubscriptionTypeStr(QXmppRosterIq::Item::SubscriptionType type) const;
    //更新条目的显示内容  
    int UpdateItemDisplay();
    //删除与本好友相关的所有条目  
    int DeleteItems();

signals:
    void ReciveMessage(CRoster* pRoster);
    //好友状态改变时触发  
    void sigChangedPresence(QXmppPresence::AvailableStatusType status);

public slots:

private:
    QString m_szJid;
    QXmppRosterIq::Item m_RosterItem;
    QXmppVCardIq m_RosterVCard;
    QXmppPresence::AvailableStatusType m_Status;//好友出席状态  

    std::list<QStandardItem*> m_lstUserListItem; //这个要交给控件释放 

    //TODO:改成动态产生  
    CFrmMessage m_Message;
    int m_nNewMessageNumber;//新消息数  
    std::list<QStandardItem*> m_lstMessageCountItem;

};

#endif // ROSTER_H
