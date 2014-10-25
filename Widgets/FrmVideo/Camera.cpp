#include "Camera.h"
#include "../../Global/Global.h"
#include <QCameraInfo>
#include <QCameraInfo>
#include <QApplication>
#include <QScreen>

CCamera::CCamera(QObject *parent) : QObject(parent),
    m_CaptureFrameProcess(this)
{
    if(!parent)
        LOG_MODEL_ERROR("Video", "CCaptureVideoFrame::CCaptureVideoFrame parent is null");

    m_pCamera = NULL;
    SetDefaultCamera();

    bool check = true;
    check = connect(&m_CaptureVideoFrame,
                    SIGNAL(sigCaptureFrame(const QVideoFrame&)),
                    &m_CaptureFrameProcess,
                    SLOT(slotCaptureFrame(const QVideoFrame&)));
    Q_ASSERT(check);

    check = connect(&m_CaptureFrameProcess,
                    SIGNAL(sigCaptureFrame(const QVideoFrame&)),
                    this,
                    SIGNAL(sigCaptureFrame(const QVideoFrame&)));
    Q_ASSERT(check);
}

CCamera::~CCamera()
{
    Stop();
}

int CCamera::Start()
{
    if(m_pCamera)
    {
        Stop();
    }

    m_pCamera = new QCamera(m_CameraPosition);
    if(!m_pCamera)
    {
        LOG_MODEL_WARNING("Video", "Open carmera fail");
        return -1;
    }

    m_pCamera->setCaptureMode(QCamera::CaptureVideo);
    m_CaptureVideoFrame.setSource(m_pCamera);

    connect(m_pCamera, SIGNAL(stateChanged(QCamera::State)), SLOT(updateCameraState(QCamera::State)));
    connect(m_pCamera, SIGNAL(error(QCamera::Error)), SLOT(displayCameraError(QCamera::Error)));

    m_pCamera->start();
    return 0;
}

int CCamera::Stop()
{
    if(m_pCamera)
    {
        m_pCamera->stop();
        m_pCamera->disconnect(this);
        delete m_pCamera;
        m_pCamera = NULL;
    }
    return 0;
}

int CCamera::SetDefaultCamera()
{
#ifdef MOBILE
    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    for(it = device.begin(); it != device.end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "Camera:%s", qPrintable(QCamera::deviceDescription(*it)));
        QCameraInfo info(*it);
        if(info.position() == QCamera::FrontFace)
        {
            m_CameraPosition = *it;
            break;
        }
    }
#else
    if (!QCamera::availableDevices().isEmpty())
        m_CameraPosition = *(QCamera::availableDevices().begin());
#endif
    return 0;
}

void CCamera::updateCameraState(QCamera::State state)
{
    LOG_MODEL_DEBUG("CaptureVideo", "CCamera::updateCameraState:%d", state);
}

void CCamera::displayCameraError(QCamera::Error e)
{
    LOG_MODEL_DEBUG("CaptureVideo", "CCamera::updateCameraState:%d", e);
}

QList<QString> CCamera::GetAvailableDevices()
{
    QList<QString> ret; 
    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    for(it = device.begin(); it != device.end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "Camera:%s", qPrintable(QCamera::deviceDescription(*it)));
        ret << QCamera::deviceDescription(*it);
    }
    return ret;
}

#ifdef MOBILE
//TODO:修改：只在相机开始时做一次。  
CCamera::Position CCamera::GetCameraPoistion()
{
    QCameraInfo info(m_CameraPosition);
    return (Position)info.position();
}
#endif

int CCamera::SetDeviceIndex(int index)
{
    LOG_MODEL_DEBUG("Video", "CCamera::SetDeviceIndex:%d", index);
    if(QCamera::availableDevices().isEmpty())
    {
        LOG_MODEL_ERROR("Video", "There isn't Camera");
        return -1;
    }

    if(!(QCamera::availableDevices().length() > index))
    {
        LOG_MODEL_ERROR("Video", "QCamera.availableDevices().length() > index");
        return -2;
    }

    m_CameraPosition = QCamera::availableDevices().at(index);

    return 0;
}

int CCamera::GetDeviceIndex()
{
    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    int i = 0;
    for(it = device.begin(); it != device.end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "Camera:%s, m_CameraPosition:%s",
                        qPrintable(QCamera::deviceDescription(*it)),
                        qPrintable(QCamera::deviceDescription(m_CameraPosition)));
        if(*it == m_CameraPosition)
            return i;
        i++;
    }
    return -1;
}

int CCamera::GetOrientation()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    int screenAngle = screen->angleBetween(screen->nativeOrientation(), screen->orientation());
    QCamera camera(m_CameraPosition);
    QCameraInfo cameraInfo(camera);
    int rotation = 0;
    rotation = (cameraInfo.orientation() - screenAngle) % 360;
    LOG_MODEL_DEBUG("main", "screen angle:%d;rotation:%d;cameraInfo.orientation():%d",
                    screenAngle, rotation, cameraInfo.orientation());
    return rotation;
}
