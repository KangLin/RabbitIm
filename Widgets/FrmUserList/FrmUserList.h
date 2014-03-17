#ifndef FRMUSERLIST_H
#define FRMUSERLIST_H

#include <QFrame>
#include <QStandardItemModel>
#include "qxmpp/QXmppRosterIq.h"
#include "qxmpp/QXmppPresence.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppLogger.h"
#include <map>
#include <QMap>
#include "Roster.h"

class CRoster;

namespace Ui {
class CFrmUserList;
}

class MainWindow;
class CFrmUserList : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmUserList(QWidget *parent = 0);
    ~CFrmUserList();

private:
    int InsertUser(QXmppRosterIq::Item rosterItem);
    int RemoveUser(QString szJid);

protected slots:
    //好友列表
    void rosterReceived();
    //好友状态改变
    void ChangedPresence(const QXmppPresence &presence);
    void vCardReceived(const QXmppVCardIq&);
    /// This signal is emitted when the roster entry of a particular bareJid is
    /// added as a result of roster push.
    void itemAdded(const QString& bareJid);

    /// This signal is emitted when the roster entry of a particular bareJid
    /// changes as a result of roster push.
    void itemChanged(const QString& bareJid);

    /// This signal is emitted when the roster entry of a particular bareJid is
    /// removed as a result of roster push.
    void itemRemoved(const QString& bareJid);

private:
    Ui::CFrmUserList *ui;

    QStandardItemModel *m_pModel;
    MainWindow *m_Parent;

    QMap<QString, CRoster*> m_Rosters;
    QMap<QString, QStandardItem*> m_Groups;
};

#endif // FRMUSERLIST_H
