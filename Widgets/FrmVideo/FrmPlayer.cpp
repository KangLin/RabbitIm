#include "FrmPlayer.h"
#include <QPainter>
#include <QImage>
#include <QThread>
#include <QEvent>
#include "DataVideoBuffer.h"
#include "../../Global/Global.h"
#include "CaptureVideoFrame.h"
#ifdef RABBITIM_USER_OPENCV
#include "CameraOpencv.h"
#else
#include "Camera.h"
#endif

CFrmPlayer::CFrmPlayer(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f)
{
    bool check = true;
    check = connect(this, SIGNAL(sigConverteToRGB32Frame(const QVideoFrame&, const QRect&)),
                    &m_FrameProcess, SLOT(slotFrameConvertedToRGB32(const QVideoFrame&, const QRect&)));
    Q_ASSERT(check);
    check = connect(&m_FrameProcess, SIGNAL(sigFrameConvertedToRGB32Frame(const QVideoFrame&)),
                        SLOT(slotPaint(const QVideoFrame&)));
    Q_ASSERT(check);
}

CFrmPlayer::~CFrmPlayer()
{
    LOG_MODEL_DEBUG("Video", "CFrmPlayer::~CFrmPlayer");
}

void CFrmPlayer::paintEvent(QPaintEvent *)
{
    if(!m_VideoFrame.isValid())
        return;
    if(!m_VideoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;
    QPainter painter(this);
    QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(m_VideoFrame.pixelFormat());
    if(QImage::Format_Invalid == f)
        return;
    QImage image(m_VideoFrame.bits(),
                 m_VideoFrame.width(),
                 m_VideoFrame.height(),
                 m_VideoFrame.bytesPerLine(),
                 f);
    //TODO:这个函数画RGB24位图在MINGW下会出错  
    painter.drawImage(this->rect(), image);
    m_VideoFrame.unmap();
}

void CFrmPlayer::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        
        break;
    }
}

void CFrmPlayer::slotPaint(const QVideoFrame &frame)
{
    m_VideoFrame = frame;
    update();
}

//从摄像头捕获的帧  
void CFrmPlayer::slotPresent(const QVideoFrame &frame)
{
    if(QVideoFrame::Format_RGB32 != frame.pixelFormat())
    {
        QRect rect = this->rect();
        emit sigConverteToRGB32Frame(frame, rect);
    }
    else
        slotPaint(frame);
}

void CFrmPlayer::mouseReleaseEvent(QMouseEvent *)
{
#ifdef DEBUG
    //TestCamera();
#endif
}

#ifdef DEBUG
int CFrmPlayer::TestCamera()
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrmPlayer", "CFrmPlayer::TestCamera threadid:0x%X",
           QThread::currentThreadId());
#endif
    //以下为视频捕获、显示测试代码  
    static CCamera *pCamera = NULL;
    if(pCamera)
    {
        pCamera->Stop();
        delete pCamera;
        pCamera = NULL;
    }

#ifdef RABBITIM_USER_OPENCV
    pCamera = new CCameraOpencv;
#else
    pCamera = new CCamera;
#endif
    //pCamera->SetDeviceIndex(0);
    pCamera->GetAvailableDevices();
    connect(pCamera, SIGNAL(sigCaptureFrame(const QVideoFrame&)),
            SLOT(slotPresent(const QVideoFrame&)));
    pCamera->Start();
    return 0;
}
#endif
