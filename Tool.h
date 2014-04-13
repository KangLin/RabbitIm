#ifndef TOOL_H
#define TOOL_H

extern "C" {
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

#include <QObject>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "opencv2/opencv.hpp"

class CTool : public QObject
{
    Q_OBJECT
public:
    explicit CTool(QObject *parent = 0);
    ~CTool();

public:
    //设置ffmpeg日志输出
    static int SetFFmpegLog();

    //格式转换
    //如果转换成功，则调用者使用完 outFrame 后，需要调用 avpicture_free(&outFrame) 释放内存
    //成功返回0，不成功返回非0
    static int ConvertFormat(/*[in]*/const QVideoFrame &inFrame,
                             /*[out]*/AVPicture &outFrame,
                             /*[in]*/int nWidth,
                             /*[in]*/int nHeight,
                             AVPixelFormat pixelFormat = AV_PIX_FMT_RGB32);
    static int ConvertFormat(/*[in]*/const QXmppVideoFrame &inFrame,
                             /*[out]*/AVPicture &outFrame,
                             /*[in]*/int nWidth,
                             /*[in]*/int nHeight,
                             AVPixelFormat pixelFormat = AV_PIX_FMT_RGB32);
    static int ConvertFormat(const AVPicture &inFrame,
                             int nInWidth, int nInHeight,
                             AVPixelFormat inPixelFormat,
                             AVPicture &outFrame,
                             int nOutWidth, int nOutHeight,
                             AVPixelFormat outPixelFormat);
    //格式映射
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(const QVideoFrame::PixelFormat format);
    static AVPixelFormat QXmppVideoFrameFormatToFFMpegPixFormat(const QXmppVideoFrame::PixelFormat format);

    //对图像进行旋转
    //参数:
    //   src:源图像
    //   _cneter:旋转中心
    //   angle:角度
    //返回值:旋转后的图像
    static cv::Mat ImageRotate(cv::Mat & src, const CvPoint &_center, double angle);
    //对YUV420sp(NV12、NV21)存储格式的图像旋转90
    static void YUV420spRotate90(uchar *des, uchar *src,int width,int height);
    //对YUV420sp(NV12、NV21)存储格式的图像做镜像
    //mode=1为x轴镜像，mode=0为y轴镜像，mode=-1为x、y轴都镜像。
    static void YUV420spMirror(uchar* dst, uchar *src, int width, int height, int mode = 0);

signals:

public slots:

};

#endif // TOOL_H
