#ifndef PLUGINPROTOCOLXMPP_H
#define PLUGINPROTOCOLXMPP_H

#include "Plugin/PluginProtocol.h"
#include "ClientXmpp.h"
#include "ManageCallXmpp.h"
#include "ManageGroupChatQxmpp.h"
#include "ManageUserQXmpp.h"

class CPluginProtocolQXMPP : public QObject, CPluginProtocol
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID rabbitim_plugin_protocol_iid FILE "ProtocolQXmpp.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(CPluginProtocol)
public:
    CPluginProtocolQXMPP(QObject *parent = 0);

    ///< 协议客户端
    virtual QSharedPointer<CClient> GetClient();
    ///< 用户信息管理对象
    virtual QSharedPointer<CManageUser> GetManageUser();
    ///< 呼叫管理对象
    virtual QSharedPointer<CManageCall> GetCall();
    ///< 组管理对象
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat();
    /**
     * 新建用户信息对象
     * @see CUser
     */
    virtual QSharedPointer<CUserInfo> NewUserInfo();

    ///属性
    ///协议ID,协议以这个ID区分
    virtual QString ID();
    //协议名称
    virtual QString Name();

private:
    QSharedPointer<CClientXmpp> m_Client;
    QSharedPointer<CManageUserQXmpp> m_User;
    QSharedPointer<CManageGroupChatQxmpp> m_GroupChat;
    QSharedPointer<CManageCall> m_Call;
};

#endif // PLUGINPROTOCOLXMPP_H
