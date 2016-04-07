#ifndef PLUGINPROTOCOLXMPP_H
#define PLUGINPROTOCOLXMPP_H

#include "Plugin/PluginProtocol.h"
#include "ClientXmpp.h"
#include "ManageGroupChatQxmpp.h"
#include "ManageUserQXmpp.h"
#ifdef RABBITIM_USE_WEBRTC
    #include "ManageCallWebrtcXmpp.h"
#else
    #include "ManageCallXmpp.h"
#endif

/**
 * @brief XMPP协议插件接口实现类  
 * 插件的生命周期:  
 * 注意插件工程.pro文件需要包含：  
    RABBITIM_PLUG_NAME=插件的类名  
    include(../ConfigProtocol.pri)

 - 初始化：Init
 - 操作：Open、About、Close
 - 清理：Clean
  * @ingroup RABBITIM_INTERFACE_PLUGINS_PROTOCOL
 */
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
