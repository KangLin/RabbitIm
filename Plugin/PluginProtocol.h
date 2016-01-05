#ifndef CPLUGINPROTOCOL_H
#define CPLUGINPROTOCOL_H

#include <QtPlugin>
#include "Client/Client.h"
#include "Manage/ManageUser.h"
#include "Manage/ManageMessageDialog.h"
#include "Widgets/FrmRecentMessage/ManageRecentMessage.h"
#include "Call/ManageCall.h"
#include <QSharedPointer>
#include "Widgets/FrmGroupChat/ManageGroupChat.h"
#include <QTranslator>

/**
 * @brief 协议插件接口类
 * 插件的生命周期:
 * 注意插件工程.pro文件需要包含：
    RABBITIM_PLUG_NAME=插件的类名
    include(../ConfigProtocol.pri)

 - 初始化：Init
 - 操作：Open、About、Close
 - 清理：Clean
 * @defgroup RABBITIM_INTERFACE_PLUGINS_PROTOCOL
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN RABBITIM_INTERFACE
 */
class RABBITIM_SHARED_LIBRARY CPluginProtocol
{
public:
    CPluginProtocol();
    ///< 协议客户端
    virtual QSharedPointer<CClient> GetClient() = 0;
    ///< 用户信息管理对象
    virtual QSharedPointer<CManageUser> GetManageUser() = 0;
    ///< 呼叫管理对象
    virtual QSharedPointer<CManageCall> GetCall() = 0;
    ///< 组管理对象
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat() = 0;
    /**
     * 新建用户信息对象
     * @see CUser
     */
    virtual QSharedPointer<CUserInfo> NewUserInfo() = 0;

    ///属性
    ///协议ID,协议以这个ID区分
    virtual QString ID() = 0;
    //协议名称
    virtual QString Name() = 0;

    ///< 初始化插件  
    virtual int InitInstance(const QString &szDir = QString());
    ///< 插件析构前做些清理工作  
    virtual int ClearInstance();

private:
    ///加载翻译
    virtual int LoadTranslate(const QString &szDir = QString());
    QSharedPointer<QTranslator> m_TranslatorPlugin;
};

#define rabbitim_plugin_protocol_iid "rabbitim.plugin.protocol"
Q_DECLARE_INTERFACE(CPluginProtocol, rabbitim_plugin_protocol_iid)
#endif // CPLUGINPROTOCOL_H
