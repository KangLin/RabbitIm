#ifndef CPLUGINAPPMOTION_H
#define CPLUGINAPPMOTION_H

#include "Plugin/PluginApp.h"
#include "FrmLbsMain.h"
#include <QSet>
#include <QSharedPointer>

/**
 * @brief 运动插件实现   
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN
 */
class CPluginAppMotion : public CPluginApp
{
    Q_OBJECT
public:
    explicit CPluginAppMotion(QObject* parent = 0);
    virtual ~CPluginAppMotion();

    virtual int Init(const QString &szId);
    virtual int Clean();

    virtual int Open(QWidget* parent = 0);
    virtual int Close();

    virtual QString Name();
    virtual QSet<QString> Group();
    virtual QIcon Icon();

private:
    QSharedPointer<CFrmLbsMain> m_Main;
};

#endif // CPLUGINAPPMOTION_H
