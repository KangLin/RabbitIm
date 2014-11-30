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
    virtual QString Subject();

    virtual int Leave();
private slots:
    /// This signal is emitted when the allowed actions change.
    void slotAllowedActionsChanged(QXmppMucRoom::Actions actions);

    /// This signal is emitted when the configuration form for the room is received.
    void slotConfigurationReceived(const QXmppDataForm &configuration);

    /// This signal is emitted when an error is encountered.
    void slotError(const QXmppStanza::Error &error);

    /// This signal is emitted once you have joined the room.
    void slotJoined();

    /// This signal is emitted if you get kicked from the room.
    void slotKicked(const QString &jid, const QString &reason);

    /// \cond
    void slotIsJoinedChanged();
    /// \endcond

    /// This signal is emitted once you have left the room.
    void slotLeft();

    /// This signal is emitted when a message is received.
    void slotMessageReceived(const QXmppMessage &message);

    /// This signal is emitted when the room's human-readable name changes.
    void slotNameChanged(const QString &name);

    /// This signal is emitted when your own nick name changes.
    void slotNickNameChanged(const QString &nickName);

    /// This signal is emitted when a participant joins the room.
    void slotParticipantAdded(const QString &jid);

    /// This signal is emitted when a participant changes.
    void slotParticipantChanged(const QString &jid);

    /// This signal is emitted when a participant leaves the room.
    void slotParticipantRemoved(const QString &jid);

    /// \cond
    void slotParticipantsChanged();
    /// \endcond

    /// This signal is emitted when the room's permissions are received.
    void slotPermissionsReceived(const QList<QXmppMucItem> &permissions);

    /// This signal is emitted when the room's subject changes.
    void slotSubjectChanged(const QString &subject);

public slots:

private:
    QXmppMucRoom *m_pRoom;
};

#endif // GROUPCHATQXMPP_H
