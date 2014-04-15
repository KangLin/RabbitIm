#include "CaptureVideoFrame.h"
#include <QCamera>
#include <QThread>

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) :
    QAbstractVideoSurface(parent)
{
    m_CaptureFrameProcess.moveToThread(&m_Thread);
    bool check = true;
    check = m_CaptureFrameProcess.connect(this,
                      SIGNAL(sigRawCaptureFrame(QVideoFrame)),
                      SLOT(slotCaptureFrame(QVideoFrame)));
    Q_ASSERT(check);
    check = connect(&m_CaptureFrameProcess,
                    SIGNAL(sigCaptureFrame(QVideoFrame)),
                    SLOT(slotCaptureFrame(QVideoFrame)));
    Q_ASSERT(check);
    m_Thread.start();
}

CCaptureVideoFrame::~CCaptureVideoFrame()
{
    m_Thread.quit();
    m_Thread.wait();
}

QList<QVideoFrame::PixelFormat> CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> lst;

    //lst.push_back(QVideoFrame::Format_YUYV);//Qt现在不支持此格式，因为Qt内部用了QImage来处理视频帧。
    lst.push_back(QVideoFrame::Format_RGB32);
    lst.push_back(QVideoFrame::Format_BGR32);

    return lst;
}

void CCaptureVideoFrame::slotCaptureFrame(const QVideoFrame &frame)
{
    emit sigCaptureFrame(frame);
}

//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度,再做Y轴镜像
//前景摄像头要逆时针旋转90度
//windows下格式是RGB32,做Y轴镜像
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
    emit sigRawCaptureFrame(frame);
    return true;
}

//根据不同的平台，设置捕获方式
bool CCaptureVideoFrame::setSource(QCamera *pCamera)
{
    bool ret = true;
#ifdef ANDROID
    //android下,目前只能用probe捕获视频
    ret = m_Probe.setSource(pCamera);
    if(ret)
    {
        connect(&m_Probe, SIGNAL(videoFrameProbed(QVideoFrame)),
                          SLOT(present(QVideoFrame)));
    }
#else
    //windows下,只能用下面方式捕获视频
    pCamera->setViewfinder(this);
#endif
    return ret;
}
