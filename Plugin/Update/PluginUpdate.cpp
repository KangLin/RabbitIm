#include "PluginUpdate.h"
#include "DlgUpdate.h"

CPluginUpdate::CPluginUpdate(QObject *parent) :
    QObject(parent), 
    CPluginApp()
{
    m_DlgUpdate = QSharedPointer<CDlgUpdate>(new CDlgUpdate);
}

int CPluginUpdate::Init(const QString &szId)
{
    Q_UNUSED(szId);
    return 0;
}

int CPluginUpdate::Clean()
{
    return 0;
}

int CPluginUpdate::Open(void *pPara, QWidget *parent)
{
    Q_UNUSED(pPara);
    Q_UNUSED(parent);
    if(m_DlgUpdate.isNull())
        m_DlgUpdate = QSharedPointer<CDlgUpdate>(new CDlgUpdate);
    if(m_DlgUpdate->isHidden())
        m_DlgUpdate->exec();
    return 0;
}

int CPluginUpdate::Close()
{
    return 0;
}

QString CPluginUpdate::ID()
{
    return "Update";
}

QString CPluginUpdate::Name()
{
    return tr("Update");
}


QSet<QString> CPluginUpdate::Group()
{
    return QSet<QString>() << tr("Tools");
}

QIcon CPluginUpdate::Icon()
{
    return QIcon(":/icon/Refresh");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Update, CPluginUpdate)
#endif // QT_VERSION < 0x050000
