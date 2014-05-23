#ifndef TOOL_H
#define TOOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

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

    /* 格式转换
     * 如果转换成功，则调用者使用完 outFrame 后，需要调用 avpicture_free(&outFrame) 释放内存
     * 成功返回0，不成功返回非0 */
    static int ConvertFormat(/*[in]*/const QVideoFrame &inFrame,     /* 要转换的帧 */
                             /*[out]*/AVPicture &outFrame,           /* 转换后的帧 */
                             /*[in]*/int nOutWidth,                  /* 转换后的帧的宽度 */
                             /*[in]*/int nOutHeight,                 /*转换后的帧的高度 */
                             /*[in]*/AVPixelFormat pixelFormat = AV_PIX_FMT_RGB32);
    static int ConvertFormat(/*[in]*/const QXmppVideoFrame &inFrame, /* 要转换的帧 */
                             /*[out]*/AVPicture &outFrame,           /* 转换后的帧 */
                             /*[in]*/int nOutWidth,                  /* 转换后的帧的宽度 */
                             /*[in]*/int nOutHeight,                 /* 转换后的帧的高度 */
                             /*[in]*/AVPixelFormat pixelFormat = AV_PIX_FMT_RGB32);
    static int ConvertFormat(const AVPicture &inFrame,               /* 要转换的帧 */
                             int nInWidth,                           /* 要转换的帧的宽度 */
                             int nInHeight,                          /* 要转换的帧的高度 */
                             AVPixelFormat inPixelFormat,            /* 要转换的帧的格式 */
                             AVPicture &outFrame,                    /* 转换后的帧 */
                             int nOutWidth,                          /* 转换后的帧的宽度 */
                             int nOutHeight,                         /* 转换后的帧的高度 */
                             AVPixelFormat outPixelFormat);          /* 转换后的帧的格式 */

    //格式映射
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(const QVideoFrame::PixelFormat format);
    static AVPixelFormat QXmppVideoFrameFormatToFFMpegPixFormat(const QXmppVideoFrame::PixelFormat format);

    /* 对图像进行旋转
     * 参数:
     *   src:源图像
     *   _cneter:旋转中心
     *   angle:角度
     * 返回值:旋转后的图像 */
    static cv::Mat ImageRotate(cv::Mat & src, const CvPoint &_center, double angle);

    /* 对YUV420sp(NV12、NV21)存储格式的图像旋转+-90度
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度
     * mode=1为顺时针旋转90度，mode=-1为逆时针旋转90度 */
    static void YUV420spRotate90(uchar *dst, const uchar *src, int srcWidth, int srcHeight, int mode);

    /* 对YUV420sp(NV12、NV21)存储格式的图像旋转90度
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度 */
    static void YUV420spRotate90(uchar *dst, const uchar *src, int srcWidth, int srcHeight);

    /* 对YUV420sp(NV12、NV21)存储格式的图像旋转-90度
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度 */
    static void YUV420spRotateNegative90(uchar *dst, const uchar *src, int srcWidth, int srcHeight);

    /* 对YUV420sp(NV12、NV21)存储格式的图像做镜像
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度
     * mode=1为x轴镜像，mode=0为y轴镜像，mode=-1为x、y轴都镜像。 */
    static void YUV420spMirror(uchar* dst, const uchar *src, int srcWidth, int srcHeight, int mode = 0);

    /* 以Y轴做镜像
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度 */
    static void YUV420spMirrorY(uchar *dst, const uchar *src, int srcWidth, int srcHeight);

    /* 以XY轴做镜像
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度 */
    static void YUV420spMirrorXY(uchar *dst, const uchar *src, int srcWidth, int srcHeight);

    /* 以X轴做镜像
     * dst:操作后的图像
     * src:要操作的图像
     * srcWidth:要操作图像的宽度
     * srcHeight:要操作图像的高度 */
    static void YUV420spMirrorX(uchar *dst, const uchar *src, int srcWidth, int srcHeight);

signals:

public slots:

};

#endif // TOOL_H
