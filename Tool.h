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
    //如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存
    //成功返回0，不成功返回非0
    static int ConvertFormat(/*[in]*/const QVideoFrame &inFrame,
                             /*[out]*/AVPicture *pOutFrame,
                             AVPixelFormat pixelFormat = AV_PIX_FMT_RGB32);

    //格式映射
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(const QVideoFrame::PixelFormat format);

signals:

public slots:

};

#endif // TOOL_H
