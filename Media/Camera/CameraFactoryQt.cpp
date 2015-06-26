#include "CameraFactoryQt.h"
#include "CameraQt.h"
#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
    #include <QCameraInfo>
#endif
#include <QList>

CCameraFactoryQt::CCameraFactoryQt() : CCameraFactory()
{
}

CCameraFactoryQt::~CCameraFactoryQt()
{
}

CCamera* CCameraFactoryQt::NewCamera(int nIndex)
{
    return new CCameraQt(nIndex);
}

/**
 * @brief 枚举系统中的摄像头设备  
 * @param info:摄像头设备信息列表  
 * @return 成功返回0;否则返回非0
 */
int CCameraFactoryQt::EnumDevice(std::vector<CCameraInfo::CamerInfo> &info)
{
    int index = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        CCameraInfo::CamerInfo ci;
        ci.szName = cameraInfo.deviceName().toStdString();
        ci.szDescription = cameraInfo.description().toStdString();
        ci.nIndex = index++;
        info.push_back(ci);
    }
#else
    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    for(it = device.begin(); it != device.end(); it++)
    {
        CCameraInfo::CamerInfo ci;
        ci.szDescription = QCamera::deviceDescription(*it).toStdString();
        ci.szName = QCamera::deviceDescription(*it).toStdString();
        ci.nIndex = index++;
        info.push_back(ci);
    }
 #endif
    return 0;
}
