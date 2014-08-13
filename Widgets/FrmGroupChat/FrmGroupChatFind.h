#ifndef FRMGROUPCHATFIND_H
#define FRMGROUPCHATFIND_H

#include <QFrame>
#include <QStandardItemModel>
#include <QList>
#include <QMenu>
#include "qxmpp/QXmppDiscoveryIq.h"
#include "Conference.h"
#include "FrmGroupChat.h"

namespace Ui {
class CFrmGroupChatFind;
}

class CFrmGroupChatFind : public QFrame
{
    Q_OBJECT
    
public:
    static CFrmGroupChatFind* Instance();

signals:
    void sigJoinedGroup(const QString &jid, CFrmGroupChat *pChat);

private:
    explicit CFrmGroupChatFind(QWidget *parent = 0);
    ~CFrmGroupChatFind();

private slots:
    void on_pbJoin_clicked();
    void on_pbRefresh_clicked();
    void on_pbCancel_clicked();
    void on_treeView_doubleClicked(const QModelIndex &index);
    
    void slotFoundServer(const QString& jid, const QString& name);
    void slotFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms);
    void slotFoundRoomInfo(const QString& jid, const QXmppDataForm& form);

    void on_listView_doubleClicked(const QModelIndex &index);
    
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void slotUpdateMenu();
    void slotNewRoom();

    void on_pbNew_clicked();
    
protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent*);

private:
    enum ENUM_ROLE{
        ROLE_JID = Qt::UserRole + 1
    };

    Ui::CFrmGroupChatFind *ui;

    QStandardItemModel *m_pModel;      //好友列表树型控件  
    QStandardItemModel *m_pListModel;  //好友列表控件  

    CConference m_Conference;
    
    QMenu m_Menu;
    int InitMenu();
};

#endif // FRMGROUPCHATFIND_H
