#ifndef PLUGINAPPFACTORY
#define PLUGINAPPFACTORY

#include "Global/Global.h"

/**
 * @class CPluginAppFactory
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN RABBITIM_INTERFACE
 * @brief 插件类厂,用于实例化指定的插件,并向CManagePlugin进行注册  
 * @see CManagePlugin 
 */
template <class T>
class CPluginAppFactory
{
public:
    CPluginAppFactory()
    {
        QSharedPointer<CPluginApp> app(new T());
        CGlobal::Instance()->GetManager()->GetManagePlugins()->RegisterPlugin(app->Name(), app);
    }
};

#endif // PLUGINAPPFACTORY

