#include "CaptureVideoFrame.h"
#include <QCamera>
#include "DataVideoBuffer.h"
#include "../../Tool.h"

#include "opencv2/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/core.hpp"

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

//捕获视频帧。windows下格式是RGB32；android下是NV21
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
#ifdef ANDROID
        int nRet = 0;
        AVPicture pic;
        int nWidth = inFrame.width(), nHeight = inFrame.height();
        AVPixelFormat pixFormat = AV_PIX_FMT_YUYV422;
        nRet = CTool::ConvertFormat(inFrame, pic, nWidth, nHeight, pixFormat);
        if(nRet)
        {
            qDebug("CTool::ConvertFormat error nRet:%d", nRet);
            break;
        }

        int nSize = avpicture_get_size(pixFormat, nWidth, nHeight);
        uchar *pBuffer = new uchar[nSize];
        if(NULL == pBuffer)
        {
            qDebug("CCaptureVideoFrame::present:new buffer fail");
            break;
        }

        do
        {
            nRet = avpicture_layout(&pic, pixFormat, nWidth, nHeight, pBuffer, nSize);
            if(nRet <= 0)
            {
                qDebug("CCaptureVideoFrame::present:avpicture_layout error.nRet:%d", nRet);
                break;
            }

            cv::Mat src(nHeight, nWidth, CV_8UC2, pBuffer), dst;
            cv::transpose(src, dst);
            cv::flip(dst, src, 1);
            CDataVideoBuffer buffer(src.data,
                                    src.total() * src.channels(),
                                    src.cols,
                                    src.rows);
            QVideoFrame outFrame(&buffer, QSize(src.cols, src.rows), QVideoFrame::Format_YUYV);
            emit CaptureFrame(outFrame);
        }while(0);

        delete pBuffer;

#else
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
#endif
    }while(0);

    inFrame.unmap();

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
