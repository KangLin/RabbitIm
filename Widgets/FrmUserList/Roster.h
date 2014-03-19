#ifndef ROSTER_H
#define ROSTER_H

#include <QObject>
#include <QStandardItemModel>
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppPresence.h"
#include <QSet>
#include "../FrmMessage/FrmMessage.h"

class CRoster : public QObject
{
    Q_OBJECT
public:
    explicit CRoster(QObject *parent = 0);
    CRoster(QString jid);
    CRoster(QString jid, QSet<QString> groups);
    ~CRoster();

    QString Name();
    QString Domain();
    QString Resouce();
    QString BareJid();
    int SetJid(QString jid);

    QSet<QString> Groups();
    int SetGroups(const QSet<QString> &groups);

    QStandardItem* GetItem();
    int ChangedPresence(QXmppPresence::Status::Type status);
    QColor GetStatusColor(QXmppPresence::Status status);

private:
    QString GetStatusText(QXmppPresence::Status status);

signals:
    void ReciveMessage(CRoster* pRoster);

public slots:

private:
    QString m_szJid;

    std::list<QStandardItem*> m_lstUserListItem; //这个要交给控件释放
    QSet<QString> m_Groups;

};

#endif // ROSTER_H
