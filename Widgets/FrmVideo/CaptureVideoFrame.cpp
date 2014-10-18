#include "CaptureVideoFrame.h"
#include <QCamera>
#include <QThread>
#include <QTime>
#include <QtDebug>
#include <QCameraInfo>
#include "FrmVideo.h"
#include "../../Global/Global.h"

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) :
    QAbstractVideoSurface(parent)
{
}

CCaptureVideoFrame::~CCaptureVideoFrame()
{
}

//选择需要捕获视频帧的格式  
QList<QVideoFrame::PixelFormat> 
CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    LOG_MODEL_DEBUG("CCaptureVideoFrame", "CCaptureVideoFrame::supportedPixelFormats handleType:%d", handleType);
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> lst;

    lst.push_back(QVideoFrame::Format_YUYV);//Qt现在不支持此格式，因为Qt内部用了QImage来处理视频帧。
    //windows 平台、linux 平台默认都支持 RGB32 格式  
    lst.push_back(QVideoFrame::Format_RGB32);
   // lst.push_back(QVideoFrame::Format_BGR32);

    return lst;
}

//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度,再做Y轴镜像
//前景摄像头要逆时针旋转90度
//windows下格式是RGB32,做Y轴镜像  
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("Video", "CCaptureVideoFrame::present:threadid:%d, preTime:%s, currTime:%s, space:%d",
           QThread::currentThreadId(),
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
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
        ret = connect(&m_Probe, 
                SIGNAL(videoFrameProbed(const QVideoFrame&)),
                SLOT(present(const QVideoFrame&)));
        Q_ASSERT(ret);
    }
    else
        LOG_MODEL_ERROR("CaptureVideo", "m_Probe.setSource fail");
#else
    //windows下,只能用下面方式捕获视频  
    pCamera->setViewfinder(this);
#endif
    return ret;
}
