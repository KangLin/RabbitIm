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

class CConvertFormat
{
public:
    CConvertFormat();
    
#ifdef RABBITIM_USE_FFMPEG
    //设置ffmpeg日志输出  
    static int SetFFmpegLog();

    /**
     * @brief 格式映射  
     * @param format 
     * @return 
     */
    static AVPixelFormat QVideoFrameFormatToFFMpegPixFormat(
            const QVideoFrame::PixelFormat format);
    static AVPixelFormat QImageFormatToFFMpegPixFormat(
            const QImage::Format format);

    static int ConvertFormat(/*[in]*/  const QVideoFrame &inFrame, /** 要转换的帧 */
                             /*[out]*/ QVideoFrame &outFrame,      /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,        /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,       /** 转换后的帧的高度 */
                             /*[in]*/  QVideoFrame::PixelFormat outPixelFormat
                                          = QVideoFrame::Format_YUYV);
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
    
private:
    /**
     * @brief 格式转换，这个函数只CTool内部调用    
     *        
     * @param inFrame：要转换的帧（注意：需要调用者用 avpicture_fill 初始化。）  
     * @param nInWidth：转换帧的宽度  
     * @param nInHeight：转换帧的高度  
     * @param inPixelFormat：转换帧的格式  
     * @param outFrame：转换后的帧（注意：需要调用者用 avpicture_alloc 或 avpicture_fill 初始化。）    
     * @param nOutWidth：转换后帧的宽度  
     * @param nOutHeight：转换后帧的高度  
     * @param outPixelFormat：转换后帧的格式  
     * @return 
     */
    static int ConvertFormat(/*[in]*/  const AVPicture &inFrame,     /** 要转换的帧 */
                             /*[in]*/  int nInWidth,                 /** 要转换的帧的宽度 */
                             /*[in]*/  int nInHeight,                /** 要转换的帧的高度 */
                             /*[in]*/  AVPixelFormat inPixelFormat,  /** 要转换的帧的格式 */
                             /*[out]*/ AVPicture &outFrame,                /** 转换后的帧 */
                             /*[in]*/  int nOutWidth,                /** 转换后的帧的宽度 */
                             /*[in]*/  int nOutHeight,               /** 转换后的帧的高度 */  
                             /*[in]*/  AVPixelFormat outPixelFormat);/** 转换后的帧的格式 */  
    
#endif //RABBITIM_USE_FFMPEG

};

#endif // CCONVERTFORMAT_H_KL_2021_10_07_
