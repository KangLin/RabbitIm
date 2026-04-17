#include <QAbstractVideoBuffer>
#include "ConvertFormatOpenCV.h"

#if OpenCV_VERSION_MAJOR == 3 || OpenCV_VERSION_MAJOR == 2
#include "opencv/cv.hpp"
#else
#include "opencv2/opencv.hpp"
#endif

CConvertFormatOpenCV::CConvertFormatOpenCV(QObject *parent) : CPluginConvertFormat(parent)
{}

QImage CConvertFormatOpenCV::onConvertFormatToRGB888(const QVideoFrame &frame)
{
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(!videoFrame.map(QVideoFrame::ReadOnly))
#else
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
#endif
    {
        qCritical("videoFrame.map fail");
        return img;
    }
    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        cv::Mat out(videoFrame.height(), videoFrame.width(), CV_8UC3, img.bits());
        switch(videoFrame.pixelFormat())
        {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_YUV420P:
#else
        case QVideoFrame::Format_YUV420P:
#endif
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_I420);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_I420);
#endif
        }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_YV12:
#else
        case QVideoFrame::Format_YV12:
#endif
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_YV12);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_YV12);
#endif
        }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_NV21:
#else
        case QVideoFrame::Format_NV21:
#endif
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_NV21);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_NV21);
#endif
        }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_NV12:
#else
        case QVideoFrame::Format_NV12:
#endif
        {
            cv::Mat in(videoFrame.height() + videoFrame.height() / 2,
                       videoFrame.width(), CV_8UC1, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_NV12);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_NV12);
#endif
        }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_YUYV:
#else
        case QVideoFrame::Format_YUYV:
#endif
        {
            //TODO： Test it
            cv::Mat in(videoFrame.height(),
                       videoFrame.width(), CV_8UC2, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_YUYV);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_YUYV);
#endif
        }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QVideoFrameFormat::PixelFormat::Format_UYVY:
#else
        case QVideoFrame::Format_UYVY:
#endif
        {
            //TODO： Test it
            cv::Mat in(videoFrame.height(),
                       videoFrame.width(), CV_8UC2, videoFrame.bits(0));
#if OpenCV_VERSION_MAJOR >= 4
            cv::cvtColor(in, out, cv::COLOR_YUV2RGB_UYVY);
#else
            cv::cvtColor(in, out, CV_YUV2RGB_UYVY);
#endif
        }
            break;

        default:
            qWarning("OpenCVConverFormatToRGB888 Don't convert format: %d",
                            videoFrame.pixelFormat());
        }

    }while(0);
    videoFrame.unmap();
    return img;
}
