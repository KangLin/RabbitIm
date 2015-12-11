#ifndef CPLUGINAPP_H
#define CPLUGINAPP_H

#include <QString>
#include <QIcon>
#include <QtPlugin>
#include <QSet>
#include <QTranslator>
#include "Global/GlobalDir.h"

/**
 * @brief 插件接口类  
 * 插件的生命周期:
 * 注意插件工程.pro文件需要包含：
    RABBITIM_PLUG_NAME=插件的类名
    include(../config.pri)
    
 - 初始化：Init
 - 操作：Open、About、Close
 - 清理：Clean
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN RABBITIM_INTERFACE
 */
class RABBITIM_SHARED_LIBRARY CPluginApp
{
public:
    CPluginApp();
    virtual ~CPluginApp();

    virtual int InitInstance(const QString &szDir = QString());

    /**
     * 登录初始化  
     * @para szId:本地用户ID  
     */
    virtual int Init(const QString &szId) = 0;
    //登出清理  
    virtual int Clean() = 0;

    ///操作  
    //打开应用  
    virtual int Open(void *pPara = 0, QWidget* parent = 0) = 0;
    //关闭应用  
    virtual int Close() = 0;
    //应用信息  
    virtual int About();
    
    ///属性  
    ///应用ID,应用以这个ID区分  
    virtual QString ID() = 0;
    //应用名称  
    virtual QString Name() = 0;
    //应用所在组  
    virtual QSet<QString> Group() = 0;
    //应用图标  
    virtual QIcon Icon();
    
private:
    ///加载翻译  
    virtual int LoadTranslate(const QString &szDir = QString());
    ///更新插件翻译资源  
    virtual int OnTranslate();
    QSharedPointer<QTranslator> m_TranslatorPlugin;
};

#define rabbitim_plugin_application_iid "rabbitim.plugin.application"
Q_DECLARE_INTERFACE(CPluginApp, rabbitim_plugin_application_iid)

#endif // CPLUGINAPP_H
