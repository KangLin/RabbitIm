#ifndef CPLUGINAPPMOTION_H
#define CPLUGINAPPMOTION_H

#include "Plugin/PluginApp.h"
#include "FrmLbsMain.h"
#include <QSet>
#include <QSharedPointer>
#include <QObject>

/**
 * @brief 运动插件实现   
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN
 */
class CPluginAppMotion : public QObject, CPluginApp
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID rabbitim_plugin_application_iid FILE "Lbs.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(CPluginApp)
    
public:
    explicit CPluginAppMotion(QObject* parent = 0);
    virtual ~CPluginAppMotion();

    virtual int Init(const QString &szId);
    virtual int Clean();

    virtual int Open(void *pPara = 0, QWidget* parent = 0);
    virtual int Close();

    virtual QString ID();
    virtual QString Name();
    virtual QSet<QString> Group();
    virtual QIcon Icon();

private:
    QSharedPointer<CFrmLbsMain> m_Main;
};

#endif // CPLUGINAPPMOTION_H
