#ifndef __VIDEOTYPE_KL__
#define __VIDEOTYPE_KL__

#pragma once
#include <string>

#define FORMAT_PACK(C1, C2, C3, C4) ( C4<<24 | C3<<16 | C2<<8 | C1 )

/**
* This enumeration uniquely identify audio sample and/or video pixel formats.
* Some well known formats are listed here. The format ids are built by
* combining four character codes, similar to FOURCC. The format id is
* extensible, as application may define and use format ids not declared
* on this enumeration.
*/
typedef enum
{
    /**
    * Video formats.
    */
    
    /**
    * 无效格式
    */
    VIDEO_FORMAT_NONE = -1,
    /**
    * 24bit RGB
    */
    VIDEO_FORMAT_RGB24 = FORMAT_PACK('R', 'G', 'B', '3'),

    /**
    * 32bit RGB with alpha channel
    */
    VIDEO_FORMAT_RGBA = FORMAT_PACK('R', 'G', 'B', 'A'),
    VIDEO_FORMAT_BGRA = FORMAT_PACK('B', 'G', 'R', 'A'),

    /**
    * Alias for VIDEO_FORMAT_RGBA
    */
    VIDEO_FORMAT_RGB32 = VIDEO_FORMAT_RGBA,

    /**
    * Device Independent Bitmap, alias for 24 bit RGB
    */
    VIDEO_FORMAT_DIB = FORMAT_PACK('D', 'I', 'B', ' '),

    /**
    * This is planar 4:4:4/24bpp RGB format, the data can be treated as
    * three planes of color components, where the first plane contains
    * only the G samples, the second plane contains only the B samples,
    * and the third plane contains only the R samples.
    */
    VIDEO_FORMAT_GBRP = FORMAT_PACK('G', 'B', 'R', 'P'),

    /**
    * This is a packed 4:4:4/32bpp format, where each pixel is encoded as
    * four consecutive bytes, arranged in the following sequence: V0, U0,
    * Y0, A0. Source:
    * http://msdn.microsoft.com/en-us/library/dd206750%28v=VS.85%29.aspx#ayuv
    */
    VIDEO_FORMAT_AYUV = FORMAT_PACK('A', 'Y', 'U', 'V'),

    /**
    * This is packed 4:2:2/16bpp YUV format, the data can be treated as
    * an array of unsigned char values, where the first byte contains
    * the first Y sample, the second byte contains the first U (Cb) sample,
    * the third byte contains the second Y sample, and the fourth byte
    * contains the first V (Cr) sample, and so forth. Source:
    * http://msdn.microsoft.com/en-us/library/dd206750%28v=VS.85%29.aspx#yuy2
    */
    VIDEO_FORMAT_YUY2 = FORMAT_PACK('Y', 'U', 'Y', '2'),

    /**
    * This format is the same as the YUY2 format except the byte order is
    * reversed -- that is, the chroma and luma bytes are flipped. If the
    * image is addressed as an array of two little-endian WORD values, the
    * first WORD contains U in the LSBs and Y0 in the MSBs, and the second
    * WORD contains V in the LSBs and Y1 in the MSBs. Source:
    * http://msdn.microsoft.com/en-us/library/dd206750%28v=VS.85%29.aspx#uyvy
    */
    VIDEO_FORMAT_UYVY = FORMAT_PACK('U', 'Y', 'V', 'Y'),

    /**
    * This format is the same as the YUY2 and UYVY format except the byte
    * order is reversed -- that is, the chroma and luma bytes are flipped.
    * If the image is addressed as an array of two little-endian WORD values,
    * the first WORD contains Y0 in the LSBs and V in the MSBs, and the second
    * WORD contains Y1 in the LSBs and U in the MSBs.
    */
    VIDEO_FORMAT_YVYU = FORMAT_PACK('Y', 'V', 'Y', 'U'),

    /**
    * This is planar 4:2:0/12bpp YUV format, the data can be treated as
    * three planes of color components, where the first plane contains
    * only the Y samples, the second plane contains only the U (Cb) samples,
    * and the third plane contains only the V (Cr) sample.
    */
    VIDEO_FORMAT_I420 = FORMAT_PACK('I', '4', '2', '0'),

    /**
    * IYUV is alias for I420.
    */
    VIDEO_FORMAT_IYUV = VIDEO_FORMAT_I420,

    /**
    * This is planar 4:2:0/12bpp YUV format, similar to I420 or IYUV but
    * the U (Cb) and V (Cr) planes order is switched, i.e: the second plane
    * contains the V (Cb) samples and the third plane contains the V (Cr)
    * samples.
    */
    VIDEO_FORMAT_YV12 = FORMAT_PACK('Y', 'V', '1', '2'),

    /**
    * This is planar 4:2:0/12bpp YUV format, the data can be treated as
    * two planes of color components, where the first plane contains
    * only the Y samples, the second plane contains interleaved
    * V (Cr) - U (Cb) samples.
    */
    VIDEO_FORMAT_NV21 = FORMAT_PACK('N', 'V', '2', '1'),

    /**
    * This is planar 4:2:2/16bpp YUV format, the data can be treated as
    * three planes of color components, where the first plane contains
    * only the Y samples, the second plane contains only the U (Cb) samples,
    * and the third plane contains only the V (Cr) sample.
    */
    VIDEO_FORMAT_I422 = FORMAT_PACK('I', '4', '2', '2'),

    /**
    * The JPEG version of planar 4:2:0/12bpp YUV format.
    */
    VIDEO_FORMAT_I420JPEG = FORMAT_PACK('J', '4', '2', '0'),

    /**
    * The JPEG version of planar 4:2:2/16bpp YUV format.
    */
    VIDEO_FORMAT_I422JPEG = FORMAT_PACK('J', '4', '2', '2'),

    /**
    * Encoded video formats
    */

    VIDEO_FORMAT_H261 = FORMAT_PACK('H', '2', '6', '1'),
    VIDEO_FORMAT_H263 = FORMAT_PACK('H', '2', '6', '3'),
    VIDEO_FORMAT_H263P = FORMAT_PACK('P', '2', '6', '3'),
    VIDEO_FORMAT_H264 = FORMAT_PACK('H', '2', '6', '4'),

    VIDEO_FORMAT_MJPEG = FORMAT_PACK('M', 'J', 'P', 'G'),
    VIDEO_FORMAT_MPEG1VIDEO = FORMAT_PACK('M', 'P', '1', 'V'),
    VIDEO_FORMAT_MPEG2VIDEO = FORMAT_PACK('M', 'P', '2', 'V'),
    VIDEO_FORMAT_MPEG4 = FORMAT_PACK('M', 'P', 'G', '4'),

} VideoFormat;

struct VideoInfo{
    VideoFormat Format;
    int nWidth, nHeight;
    unsigned int nRatio;
    //std::string szName;//用函数VideoFormatToName得到  
};

inline std::string VideoFormatToName(VideoFormat vf)
{
    char buf[5];
    buf[3] = (char)((vf >> 24) & 0xFF);
    buf[2] = (char)((vf >> 16) & 0xFF);
    buf[1] = (char)((vf >>  8) & 0xFF);
    buf[0] = (char)((vf >>  0) & 0xFF);
    buf[4] = '\0';
    return buf;
}

#include "../../Global/Global.h"
/*无qt环境时使用  
#include <QDebug>
#define LOG_MODEL_ERROR(model, fmt, ...) qDebug(fmt, ##__VA_ARGS__) 
#define LOG_MODEL_DEBUG(model, fmt, ...) qDebug(fmt, ##__VA_ARGS__) //*/

#endif
