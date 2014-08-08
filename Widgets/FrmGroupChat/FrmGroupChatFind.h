#ifndef FRMGROUPCHATFIND_H
#define FRMGROUPCHATFIND_H

#include <QFrame>
#include <QStandardItemModel>
#include <QList>
#include "qxmpp/QXmppDiscoveryIq.h"
#include "Conference.h"

namespace Ui {
class CFrmGroupChatFind;
}

class CFrmGroupChatFind : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmGroupChatFind(QWidget *parent = 0);
    ~CFrmGroupChatFind();
    
protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pbJoin_clicked();
    void on_pbRefresh_clicked();
    void on_pbCancel_clicked();
    void on_treeView_doubleClicked(const QModelIndex &index);
    
    void slotFoundServer(const QString& jid, const QString& name);
    void slotFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms);

signals:
    void sigJoinGroup(const QString &jid);

private:
    Ui::CFrmGroupChatFind *ui;

    QStandardItemModel *m_pModel;      //好友列表树型控件  
    QStandardItemModel *m_pListModel;  //好友列表控件  

    CConference m_Conference;
};

#endif // FRMGROUPCHATFIND_H
