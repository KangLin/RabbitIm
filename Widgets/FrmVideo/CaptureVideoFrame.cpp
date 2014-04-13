#include "CaptureVideoFrame.h"
#include <QCamera>
#include "DataVideoBuffer.h"
#include "../../Tool.h"

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) :
    QAbstractVideoSurface(parent)
{
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

#ifdef ANDROID
//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度,再做Y轴镜像
//前景摄像头要逆时针旋转90度
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
    if(frame.pixelFormat() != QVideoFrame::Format_NV21)
    {
        qDebug("CCaptureVideoFrame::present:don't Format_NV21");
        emit CaptureFrame(frame);
        return 0;
    }

    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        qDebug("CCaptureVideoFrame::present map error");
        return false;
    }

    do{
        //背景摄像头要顺时针旋转90度,再做Y轴镜像
        //前景摄像头要逆时针旋转90度
        int nWidth = inFrame.width();
        int nHeight = inFrame.height();
        static int nSize = 0;
        static uchar *pBuf = NULL;
        static uchar *pRotate = NULL;
        if(inFrame.mappedBytes() != nSize
                || NULL == pBuf
                || NULL == pRotate)
        {
            nSize = inFrame.mappedBytes();
            if(pBuf) delete pBuf;
            if(pRotate) delete pRotate;
            pBuf = new uchar[nSize];
            pRotate = new uchar[nSize];
        }

        if(NULL == pBuf || NULL == pRotate)
        {
            qDebug("CCaptureVideoFrame::present null pointer");
            break;
        }

        CTool::YUV420spRotate90(pRotate, inFrame.bits(), nWidth, nHeight, 1);
        CTool::YUV420spMirror(pBuf, pRotate, nHeight, nWidth, 0);

        CDataVideoBuffer buffer(pBuf,
                                nSize,
                                nHeight,
                                nWidth);
        QVideoFrame outFrame(&buffer, QSize( nHeight, nWidth), QVideoFrame::Format_NV21);
        emit CaptureFrame(outFrame);

    }while(0);

    inFrame.unmap();

    return true;
}
#else
//捕获视频帧。windows下格式是RGB32,做Y轴镜像
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        qDebug("CCaptureVideoFrame::present map error");
        return false;
    }

    do{
        //windows下要镜像，android下要旋转90度
        if(inFrame.pixelFormat() != QVideoFrame::Format_RGB32)
        {
            emit CaptureFrame(frame);
        }
        else
        {
            /*用QImage做图像镜像
            QImage img(inFrame.bits(), inFrame.width(), inFrame.height(), QImage::Format_RGB32);
            img = img.mirrored(true, false);
            QVideoFrame outFrame(img);//*/

            //*用opencv库做图像镜像
            cv::Mat src(inFrame.height(), inFrame.width(), CV_8UC4, inFrame.bits());
            cv::Mat dst;
            cv::flip(src, dst, 1); //最后一个参数>0为x轴镜像，=0为y轴镜像，，<0x,y轴都镜像。
            CDataVideoBuffer buffer(dst.data,
                                    dst.total() * dst.channels(), //dst.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()
                                    dst.cols,
                                    dst.rows);
            QVideoFrame outFrame(&buffer,
                                 QSize(dst.cols,
                                       dst.rows),
                                 inFrame.pixelFormat());//*/
            emit CaptureFrame(outFrame);//*/
        }

    }while(0);

    inFrame.unmap();

    return true;
}
#endif

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
