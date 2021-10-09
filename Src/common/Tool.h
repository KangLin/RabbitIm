#ifndef TOOL_H
#define TOOL_H

//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//#pragma execution_character_set("utf-8")
//#endif

#ifdef RABBITIM_USE_FFMPEG
extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#undef PixelFormat
}
#endif

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>
#include <QVideoFrame>
#include <QFileDialog>

#ifdef RABBITIM_USE_OPENCV
    #include "opencv2/opencv.hpp"
#endif
//#include "Media/Camera/VideoFrame.h"
#include "Global/Global.h"

/**
  * @defgroup RABBITIM_TOOL 工具类模块  
  */
class RABBITIM_SHARED_LIBRARY CVideoFrame;
/**
 * @brief 工具类。提供一此工具函数  
 * @ingroup RABBITIM_TOOL
 */
class RABBITIM_SHARED_LIBRARY CTool : public QObject
{
    Q_OBJECT
public:
    explicit CTool(QObject *parent = 0);
    ~CTool();

public:

#ifdef RABBITIM_USE_OPENCV
    /** 
     * @brief 对图像进行旋转  
     * @param src:源图像
     * @param _cneter:旋转中心
     * @param angle:角度
     * @param scale:缩放  
     * 返回值:旋转后的图像  */
    static cv::Mat ImageRotate(cv::Mat & src,
                               const cv::Point &_center,
                               double angle, double scale = 1);
#endif
    
    static int ImageRotate(const QVideoFrame& inFrame, QVideoFrame &outFrame, double nAngle = 0);
    static int ImageTransformed(const QVideoFrame &inFrame, QVideoFrame &outFrame, double nScale = 1, double nAngle = 0);
    
    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转+-90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度
     * @param mode=1为顺时针旋转90度，mode=-1为逆时针旋转90度  
     */
    static void YUV420spRotate90(uchar *dst,
                                 const uchar *src, int srcWidth,
                                 int srcHeight, int mode);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spRotate90(uchar *dst, const uchar *src,
                                 int srcWidth, int srcHeight);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像旋转-90度
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spRotateNegative90(uchar *dst, const uchar *src,
                                         int srcWidth, int srcHeight);

    /** 
     * @brief 对YUV420sp(NV12、NV21)存储格式的图像做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度
     * @param mode=1为x轴镜像，mode=0为y轴镜像，mode=-1为x、y轴都镜像。 
     */
    static void YUV420spMirror(uchar* dst, const uchar *src,
                               int srcWidth, int srcHeight, int mode = 0);

    /** 
     * @brief 以Y轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度   
     */
    static void YUV420spMirrorY(uchar *dst, const uchar *src,
                                int srcWidth, int srcHeight);

    /** 
     * @brief 以XY轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度  
     */
    static void YUV420spMirrorXY(uchar *dst, const uchar *src,
                                 int srcWidth, int srcHeight);

    /** 
     * @brief 以X轴做镜像
     * @param dst:操作后的图像
     * @param src:要操作的图像
     * @param srcWidth:要操作图像的宽度
     * @param srcHeight:要操作图像的高度   
     */
    static void YUV420spMirrorX(uchar *dst, const uchar *src,
                                int srcWidth, int srcHeight);

    static bool isImageFile(const QString& szFile);///<判断文件是否是图片，根据扩展名进行判断  
    
    /**
     * @brief 删除指定的目录  
     *
     * @param dirName 要删除的目录名  
     * @return bool 成功返回 true，否则返回 false  
     */
    static bool removeDirectory(QString dirName);
    
    /**
     * @brief 调整窗口的位置，手机上，让窗口全屏，在pc上，把窗口调整到屏幕中间  
     *
     * @fn SetWindowsGeometry
     * @param pWindow
     * @return int
     */
    static int SetWindowsGeometry(QWidget* pWindow);
    
    static QString FileDialog(QWidget* pParent, const QString &szDir,
                              const QString &szFilter,
                              const QString &szTilte = tr("Open file"), const QFileDialog::AcceptMode mode = QFileDialog::AcceptOpen);
    static std::string DoubleToString(double d);
    /**
     * 得到文件的校验和  
     */
    static QByteArray GetFileMd5Sum(QString filePath);
    static QString GetFileMd5SumString(QString filePath);

    /**
     * @brief 合并图片
     * @param szFile1
     * @param szFile2
     * @return 
     */
    static QPixmap MergePixmap(const QString& szFile1, const QString& szFile2);
    static int MergePixmap(QPixmap &src1, const QPixmap &src2);
    /**
     * @brief 合并状态图片到头像
     * @param src1
     * @param src2
     * @return 
     */
    static int MergeAvatarStatus(QPixmap &src1, const QPixmap& src2);

    static QImage ConvertToGray(QImage image);
    
    /**
     * @brief 禁用锁屏  
     * @param bWake：true，禁用锁屏，false，允许锁屏  
     * @return 成功返回 true，否则返回 false
     */
    static bool EnableWake(bool bWake = true);

signals:

public slots:

};

#endif // TOOL_H
