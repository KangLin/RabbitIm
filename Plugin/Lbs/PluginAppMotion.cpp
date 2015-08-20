#include "PluginAppMotion.h"
#include "Plugin/ManagePlugin.h"
#include <QMessageBox>
#include "Global/Log.h"

CPluginAppMotion::CPluginAppMotion(QObject *parent) : CPluginApp(parent)
{
}

CPluginAppMotion::~CPluginAppMotion()
{
    LOG_MODEL_DEBUG("CPluginAppMotion", "CPluginAppMotion::~CPluginAppMotion");
}

int CPluginAppMotion::Init(const QString &szId)
{
    return 0;
}

int CPluginAppMotion::Clean()
{
    if(!m_Main.isNull())
    {  
        m_Main->close();
        m_Main.clear();
    }
    return 0;
}

int CPluginAppMotion::Open(QWidget *parent)
{
    if(m_Main.isNull())
        m_Main = QSharedPointer<CFrmLbsMain>(new CFrmLbsMain());
    if(!m_Main.isNull())
        m_Main->show();
    return 0;
}

int CPluginAppMotion::Close()
{
    if(!m_Main.isNull())
    {  
        m_Main->close();
    }
    return 0;
}

QString CPluginAppMotion::Name()
{
    return tr("Motion");
}

QSet<QString> CPluginAppMotion::Group()
{
    return QSet<QString>() << tr("Motion") << tr("Lbs");
}

QIcon CPluginAppMotion::Icon()
{
    return QIcon(":/png/motion");
}
