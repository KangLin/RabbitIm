#ifndef CPLUGINAPP_H
#define CPLUGINAPP_H

#include <QString>
#include <QWidget>
#include <QObject>
#include <QIcon>

/**
 * @brief 插件接口类  
 * 插件的生命周期:
 - 初始化：Init
 - 操作：Open、About、Close
 - 清理：Clean
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN RABBITIM_INTERFACE
 */
class CPluginApp : public QObject
{
    Q_OBJECT
public:
    explicit CPluginApp(QObject* parent = 0);
    virtual ~CPluginApp();

    //登录初始化  
    virtual int Init(const QString &szId) = 0;
    //登出清理  
    virtual int Clean() = 0;

    ///操作  
    //打开应用  
    virtual int Open(QWidget* parent = 0) = 0;
    //关闭应用  
    virtual int Close() = 0;
    //应用信息  
    virtual int About();
    
    ///属性  
    //应用名称  
    virtual QString Name() = 0;
    //应用所在组  
    virtual QSet<QString> Group() = 0;
    //应用图标  
    virtual QIcon Icon();
};

#endif // CPLUGINAPP_H
