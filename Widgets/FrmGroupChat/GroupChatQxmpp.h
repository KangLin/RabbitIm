#ifndef GROUPCHATQXMPP_H
#define GROUPCHATQXMPP_H

#include <QObject>
#include "qxmpp/QXmppMucManager.h"
#include "GroupChat.h"

class CGroupChatQxmpp : public CGroupChat
{
    Q_OBJECT
public:
    explicit CGroupChatQxmpp(QXmppMucRoom *pRoom, QObject *parent = 0);

    virtual QString Id();
    virtual QString ShowName();
    virtual QString Nick();
    virtual int Leave();
private slots:
    void slotJoined();

public slots:

private:
    QXmppMucRoom *m_pRoom;
};

#endif // GROUPCHATQXMPP_H
