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

int CPluginQRCode::Open(QWidget *parent)
{
    m_FrmScanQRCode.show();
    return 0;
}

int CPluginQRCode::Close()
{
    m_FrmScanQRCode.close();
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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(QRCode, CPluginQRCode)
#endif // QT_VERSION < 0x050000

