qxmpp 登录后,qxmpp会自动发送请求好友列表消息:


    /// Upon XMPP connection, request the roster.
    ///
    void QXmppRosterManager::_q_connected()
    {
        QXmppRosterIq roster;
        roster.setType(QXmppIq::Get);
        roster.setFrom(client()->configuration().jid());
        d->rosterReqId = roster.id();
        if (client()->isAuthenticated())
            client()->sendPacket(roster);
    }


接受到好友列表消息后触发消息  `: QXmppRosterManager::rosterReceived() 

