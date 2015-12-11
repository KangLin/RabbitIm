#include "CPluginQRCode.h"

CPluginQRCode::CPluginQRCode(QObject *parent) :
    QObject(parent),
    CPluginApp()
{
}

int CPluginQRCode::Init(const QString &szId)
{
    Q_UNUSED(szId);
    return 0;
}

int CPluginQRCode::Clean()
{
    return 0;
}

int CPluginQRCode::Open(void *pPara, QWidget *parent)
{
    Q_UNUSED(pPara);
    Q_UNUSED(parent);
    if(m_FrmScanQRCode.isNull())
        m_FrmScanQRCode = QSharedPointer<CFrmScanQRcode>(new CFrmScanQRcode());
    if(!m_FrmScanQRCode.isNull())
        m_FrmScanQRCode->show();
    return 0;
}

int CPluginQRCode::Close()
{
    if(!m_FrmScanQRCode.isNull())
    {  
        m_FrmScanQRCode->close();
        m_FrmScanQRCode.clear();
    }
    return 0;
}

QString CPluginQRCode::ID()
{
    return "QRCode";
}

QString CPluginQRCode::Name()
{
    return tr("QRcode");
}

QSet<QString> CPluginQRCode::Group()
{
    return QSet<QString>() << tr("Tools");
}

//应用图标  
QIcon CPluginQRCode::Icon()
{
    return QIcon(":/icon/Scan");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(QRCode, CPluginQRCode)
#endif // QT_VERSION < 0x050000

