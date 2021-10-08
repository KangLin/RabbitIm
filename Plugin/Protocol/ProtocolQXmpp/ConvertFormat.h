#ifndef CCONVERTFORMAT_H_KL_2021_10_07_
#define CCONVERTFORMAT_H_KL_2021_10_07_

#pragma once

#ifdef RABBITIM_USE_FFMPEG
extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#undef PixelFormat
}
#endif

#include "QXmppRtpChannel.h"
#include <QVideoFrame>
#include "Tool.h"

class CConvertFormat : public CTool
{
    Q_OBJECT
public:
    CConvertFormat();
    
#ifdef RABBITIM_USE_FFMPEG

    static int ConvertFormat(/*[in]*/  const QVideoFrame &inFrame, /** 要转换的帧 */
                             /*[out]*/ QXmppVideoFrame &outFrame,  /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,        /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,       /** 转换后的帧的高度 */
                             /*[in]*/  QVideoFrame::PixelFormat outPixelFormat
                                          = QVideoFrame::Format_YUYV);
    static int ConvertFormat(/*[in]*/  const QXmppVideoFrame &inFrame, /** 要转换的帧 */
                             /*[out]*/ QVideoFrame &outFrame,          /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,            /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,           /** 转换后的帧的高度 */
                             /*[in]*/  QVideoFrame::PixelFormat outPixelFormat
                                          = QVideoFrame::Format_RGB32);
    static AVPixelFormat QXmppVideoFrameFormatToFFMpegPixFormat(
            const QXmppVideoFrame::PixelFormat format);
    static QXmppVideoFrame::PixelFormat QVideoFrameFormatToQXmppVideoFrameFormat(
            const QVideoFrame::PixelFormat format);
    static QVideoFrame::PixelFormat QXmppVideoFrameFormatToQVideoFrameFormat(
            const QXmppVideoFrame::PixelFormat format);
    
#endif //RABBITIM_USE_FFMPEG

};

#endif // CCONVERTFORMAT_H_KL_2021_10_07_
