#ifndef FRMGROUPCHAT_H
#define FRMGROUPCHAT_H

#include <QFrame>
#include <QStandardItem>
#include "qxmpp/QXmppMucManager.h"

namespace Ui {
class CFrmGroupChat;
}

class CFrmGroupChat : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmGroupChat(QXmppMucRoom *room, QWidget *parent = 0);
    ~CFrmGroupChat();

    QStandardItem* GetItem();

private slots:
    void slotJoined();

private:
    Ui::CFrmGroupChat *ui;

    QXmppMucRoom* m_pRoom;
    QStandardItem* m_pItem;
};

#endif // FRMGROUPCHAT_H
