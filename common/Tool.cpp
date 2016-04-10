#include "Tool.h"
#include "Global/Global.h"
#include <QFileInfo>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <sstream>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QPainter>

#ifdef ANDROID
    #include <QtAndroidExtras/QAndroidJniObject>
#endif
#ifdef WINDOWS
    #include <windows.h>
    #pragma comment(lib, "User32.lib")
#endif

CTool::CTool(QObject *parent) :
    QObject(parent)
{
}

CTool::~CTool()
{
}

//设置日志的回调函数  
void Log(void*, int, const char* fmt, va_list vl)
{
    LOG_MODEL_DEBUG("ffmpeg", fmt, vl);
}

#ifdef RABBITIM_USE_FFMPEG
int CTool::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数  
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CTool::QVideoFrameFormatToFFMpegPixFormat(
        const QVideoFrame::PixelFormat format)
{
    if(QVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QVideoFrame::Format_BGR24 == format)
        return AV_PIX_FMT_BGR24;
    else if(QVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QVideoFrame::Format_NV21 == format)
        return AV_PIX_FMT_NV21;
    else if(QVideoFrame::Format_NV12 == format)
        return AV_PIX_FMT_NV12;
    else if(QVideoFrame::Format_YUV444 == format)
        return AV_PIX_FMT_YUYV422;

    return AV_PIX_FMT_NONE;
}

AVPixelFormat CTool::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

#ifdef RABBITIM_USE_QXMPP
AVPixelFormat CTool::QXmppVideoFrameFormatToFFMpegPixFormat(
        const QXmppVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return AV_PIX_FMT_YUV420P;
    else
        return AV_PIX_FMT_NONE;
}

QXmppVideoFrame::PixelFormat CTool::QVideoFrameFormatToQXmppVideoFrameFormat(
        const QVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return QXmppVideoFrame::Format_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return QXmppVideoFrame::Format_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return QXmppVideoFrame::Format_YUYV;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return QXmppVideoFrame::Format_UYVY;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return QXmppVideoFrame::Format_YUV420P;
    else
        return QXmppVideoFrame::Format_Invalid;
}

int CTool::ConvertFormat(const QXmppVideoFrame &inFrame,
                         QVideoFrame &outFrame,
                         int nOutWidth,
                         int nOutHeight,
                         QVideoFrame::PixelFormat outPixelFormat)
{
    int nRet = 0;
    AVPicture inPic, outPic;
    nRet = avpicture_fill(&inPic, (uint8_t*)inFrame.bits(),
                          QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                          inFrame.width(),
                          inFrame.height());
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
        return nRet;
    }

    int nByte = avpicture_fill(&outPic, NULL,
                          QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                          nOutWidth,
                          nOutHeight);
    if(nByte < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
        return -2;
    }
    QVideoFrame out(nByte, QSize(nOutWidth, nOutHeight), outPic.linesize[0], outPixelFormat);
    if(!out.map(QAbstractVideoBuffer::WriteOnly))
    {
        LOG_MODEL_ERROR("CTool", "outFrame map is fail");
        return -3;
    }
    do{
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(out.pixelFormat()),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_file is fail:%d", nRet);
            nRet = -4;
            break;
        }
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        if(!nRet)
            outFrame = out;
    }while(0);
    out.unmap();
    return nRet;
}

#endif

int CTool::ConvertFormat(const QVideoFrame &inFrame,
                         QVideoFrame &outFrame,
                         int nOutWidth,
                         int nOutHeight,
                         QVideoFrame::PixelFormat outPixelFormat)
{
    int nRet = 0;
    QVideoFrame in(inFrame);
    if(!in.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("CTool", "map is fail");
        return -1;
    }

    do{
        AVPicture inPic, outPic;
        nRet = avpicture_fill(&inPic, in.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(in.pixelFormat()),
                              in.width(),
                              in.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
            return nRet;
        }
        /*int nByte = avpicture_get_size(QVideoFrameFormatToFFMpegPixFormat(outPixelFormat), nOutWidth, nOutHeight);
        if(nByte < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
            nRet = -2;
            break;
        }*/
        int nByte = avpicture_fill(&outPic, NULL,
                              QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                              nOutWidth,
                              nOutHeight);
        if(nByte < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
            nRet = -2;
            break;
        }
        QVideoFrame out(nByte, QSize(nOutWidth, nOutHeight), outPic.linesize[0], outPixelFormat);
        if(!out.map(QAbstractVideoBuffer::WriteOnly))
        {
            LOG_MODEL_ERROR("CTool", "outFrame map is fail");
            nRet = -3;
            break;
        }
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(out.pixelFormat()),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
            nRet = -4;
            break;
        }
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        out.unmap();
        if(nRet)
        {
            LOG_MODEL_ERROR("CTool", "CTool::ConvertFormat is fail");
            break;
        }
        outFrame = out;
        nRet = 0;
    }while(0);

    in.unmap();
    return nRet;
}

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存  
//成功返回0，不成功返回非0  
int CTool::ConvertFormat(/*[in]*/ const AVPicture &inFrame,
                         /*[in]*/ int nInWidth,
                         /*[in]*/ int nInHeight,
                         /*[in]*/ AVPixelFormat inPixelFormat,
                         /*[out]*/AVPicture &outFrame,
                         /*[in]*/ int nOutWidth,
                         /*[in]*/ int nOutHeight,
                         /*[in]*/ AVPixelFormat outPixelFormat)
{
    int nRet = 0;
    struct SwsContext* pSwsCtx = NULL;
    
    //分配输出空间  
    /*nRet = avpicture_alloc(&outFrame, outPixelFormat, nOutWidth, nOutHeight);
    if(nRet)
    {
        LOG_MODEL_ERROR("Tool", "avpicture_alloc fail:%x", nRet);
        return nRet;
    }*/
    
    if(inPixelFormat == outPixelFormat
            && nInWidth == nOutWidth
            && nInHeight == nOutHeight)
    {
        av_picture_copy(&outFrame, &inFrame, inPixelFormat,
                        nInWidth, nInHeight);
        return 0;
    }
    
    //设置图像转换上下文  
    pSwsCtx = sws_getCachedContext (NULL,
                                    nInWidth,                //源宽度  
                                    nInHeight,               //源高度  
                                    inPixelFormat,           //源格式  
                                    nOutWidth,               //目标宽度  
                                    nOutHeight,              //目标高度  
                                    outPixelFormat,          //目的格式  
                                    SWS_FAST_BILINEAR,       //转换算法  
                                    NULL, NULL, NULL);
    if(NULL == pSwsCtx)
    {
        LOG_MODEL_ERROR("Tool", "sws_getContext false");
        return -3;
    }
    
    //进行图片转换  
    nRet = sws_scale(pSwsCtx,
                     inFrame.data, inFrame.linesize,
                     0, nInHeight,
                     outFrame.data, outFrame.linesize);
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("Tool", "sws_scale fail:%x", nRet);
    }
    else
    {
        nRet = 0;
    }
    
    sws_freeContext(pSwsCtx);
    return nRet;
}

#endif

#ifdef RABBITIM_USE_OPENCV
cv::Mat CTool::ImageRotate(cv::Mat & src, const cv::Point &_center,
                           double angle, double scale)
{
    cv::Point2f center;
    center.x = float(_center.x);
    center.y = float(_center.y);
    
    //计算二维旋转的仿射变换矩阵  
    cv::Mat M = cv::getRotationMatrix2D(center, angle, scale);
    
    // rotate
    cv::Mat dst;
    cv::warpAffine(src, dst, M, cv::Size(src.cols, src.rows),
                   cv::INTER_LINEAR);
    return dst;
}
#endif

void CTool::YUV420spRotate90(uchar *dst, const uchar *src,
                             int srcWidth, int srcHeight)
{
    static int nWidth = 0, nHeight = 0;
    static int wh = 0;
    static int uvHeight = 0;
    if(srcWidth != nWidth || srcHeight != nHeight)
    {
        nWidth = srcWidth;
        nHeight = srcHeight;
        wh = srcWidth * srcHeight;
        uvHeight = srcHeight >> 1;//uvHeight = height / 2
    }
    
    //旋转Y  
    int k = 0;
    for(int i = 0; i < srcWidth; i++) {
        int nPos = 0;
        for(int j = 0; j < srcHeight; j++) {
            dst[k] = src[nPos + i];
            k++;
            nPos += srcWidth;
        }
    }
    
    for(int i = 0; i < srcWidth; i+=2){
        int nPos = wh;
        for(int j = 0; j < uvHeight; j++) {
            dst[k] = src[nPos + i];
            dst[k + 1] = src[nPos + i + 1];
            k += 2;
            nPos += srcWidth;
        }
    }
    return;
}

void CTool::YUV420spRotateNegative90(uchar *dst, const uchar *src,
                                     int srcWidth, int height)
{
    static int nWidth = 0, nHeight = 0;
    static int wh = 0;
    static int uvHeight = 0;
    if(srcWidth != nWidth || height != nHeight)
    {
        nWidth = srcWidth;
        nHeight = height;
        wh = srcWidth * height;
        uvHeight = height >> 1;//uvHeight = height / 2
    }
    
    //旋转Y  
    int k = 0;
    for(int i = 0; i < srcWidth; i++){
        int nPos = srcWidth - 1;
        for(int j = 0; j < height; j++)
        {
            dst[k] = src[nPos - i];
            k++;
            nPos += srcWidth;
        }
    }
    
    for(int i = 0; i < srcWidth; i+=2){
        int nPos = wh + srcWidth - 1;
        for(int j = 0; j < uvHeight; j++) {
            dst[k] = src[nPos - i - 1];
            dst[k + 1] = src[nPos - i];
            k += 2;
            nPos += srcWidth;
        }
    }
    
    return;
}

void CTool::YUV420spRotate90(uchar *dst, const uchar *src, int srcWidth,
                             int height, int mode)
{
    switch (mode) {
    case 1:
        YUV420spRotate90(dst, src, srcWidth, height);
        break;
    case -1:
        YUV420spRotateNegative90(dst, src, srcWidth, height);
        break;
    default:
        break;
    }
    return;
}

//以Y轴做镜像  
void CTool::YUV420spMirrorY(uchar *dst, const uchar *src, int srcWidth,
                            int srcHeight)
{
    //镜像Y  
    int k = 0;
    int nPos = -1;
    for(int j = 0; j < srcHeight; j++) {
        nPos += srcWidth;
        for(int i = 0; i < srcWidth; i++)
        {
            dst[k] = src[nPos - i];
            k++;
        }
    }
    
    int uvHeight = srcHeight >> 1; // uvHeight = height / 2
    for(int j = 0; j < uvHeight; j ++) {
        nPos += srcWidth;
        for(int i = 0; i < srcWidth; i += 2)
        {
            dst[k] = src[nPos - i - 1];
            dst[k+1] = src[nPos - i];
            k+=2;
        }
    }
}

//以XY轴做镜像  
void CTool::YUV420spMirrorXY(uchar *dst, const uchar *src,
                             int width, int srcHeight)
{
    static int nWidth = 0, nHeight = 0;
    static int wh = 0;
    static int nUVPos = 0;
    static int uvHeight = 0;
    if(width != nWidth || srcHeight != nHeight)
    {
        nWidth = width;
        nHeight = srcHeight;
        
        wh = width * srcHeight;
        uvHeight = srcHeight >> 1; //uvHeight = height / 2
        nUVPos = wh + uvHeight * width - 1;
    }
    
    //镜像Y  
    int k = 0;
    int nPos = wh - 1;
    for(int j = 0; j < srcHeight; j++) {
        for(int i = 0; i < width; i++)
        {
            dst[k] = src[nPos - i];
            k++;
        }
        nPos -= width;
    }
    
    nPos = nUVPos;
    for(int j = 0; j < uvHeight; j ++) {
        for(int i = 0; i < width; i += 2)
        {
            dst[k] = src[nPos - i - 1];
            dst[k + 1] = src[nPos - i];
            k += 2;
        }
        nPos -= width;
    }
}

//以X轴做镜像  
void CTool::YUV420spMirrorX(uchar *dst, const uchar *src,
                            int width, int srcHeight)
{
    static int nWidth = 0, nHeight = 0;
    static int wh = 0;
    static int nUVPos = 0;
    static int uvHeight = 0;
    if(width != nWidth || srcHeight != nHeight)
    {
        nWidth = width;
        nHeight = srcHeight;
        
        wh = width * srcHeight;
        uvHeight = srcHeight >> 1; //uvHeight = height / 2
        nUVPos = wh + uvHeight * width;
    }
    
    //镜像Y  
    int k = 0;
    int nPos = wh - 1;
    for(int j = 0; j < srcHeight; j++) {
        nPos -= width;
        for(int i = 0; i < width; i++)
        {
            dst[k] = src[nPos + i];
            k++;
        }
    }
    
    nPos = nUVPos;
    for(int j = 0; j < uvHeight; j ++) {
        nPos -= width;
        for(int i = 0; i < width; i += 2)
        {
            dst[k] = src[nPos + i];
            dst[k+1] = src[nPos + i + 1];
            k+=2;
        }
    }
}

void CTool::YUV420spMirror(uchar *dst, const uchar *src, int srcWidth,
                           int srcHeight, int mode)
{
    switch (mode) {
    case 0:
        return YUV420spMirrorY(dst, src, srcWidth, srcHeight);
        break;
    case 1:
        return YUV420spMirrorX(dst, src, srcWidth, srcHeight);
    case -1:
        return YUV420spMirrorXY(dst, src, srcWidth, srcHeight);
    default:
        break;
    }
}

bool CTool::isImageFile(const QString &szFile)
{
    QStringList imgSuffix;
    imgSuffix << "png" << "gif" << "ico" << "bmp" << "jpg";
    QFileInfo info(szFile);
    QString suffix = info.suffix().toLower();
    if(imgSuffix.indexOf(suffix) != -1)
        return true;
    return false;
}

bool CTool::removeDirectory(QString dirName)
{
    QDir dir(dirName);
    QString tmpdir ="";
    if(!dir.exists()){
        return false;
    }
    
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName()=="."|| fileInfo.fileName()=="..")
            continue;
        
        if(fileInfo.isDir()){
            tmpdir = dirName +("/")+ fileInfo.fileName();
            removeDirectory(tmpdir);
            dir.rmdir(fileInfo.fileName());/**< 移除子目录 */
        }
        else if(fileInfo.isFile()){
            QFile tmpFile(fileInfo.fileName());
            dir.remove(tmpFile.fileName());/**< 删除临时文件 */
        }
    }
    
    dir.cdUp();            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
    if(dir.exists(dirName)){
        if(!dir.rmdir(dirName))
            return false;
    }
    return true;
}

int CTool::SetWindowsGeometry(QWidget *pWindow)
{
    /*
    QDesktopWidget *pDesk = QApplication::desktop();
    LOG_MODEL_DEBUG("CTool", "availableGeometry:%d,%d; screenGeometry:%d,%d; geometry:%d,%d",
                    pDesk->availableGeometry().width(),
                    pDesk->availableGeometry().height(),
                    pDesk->screenGeometry().width(),
                    pDesk->screenGeometry().height(),
                    pDesk->geometry().width(),
                    pDesk->geometry().height());
#ifdef MOBILE
    pWindow->setGeometry(pDesk->availableGeometry());
#else
    pWindow->move((pDesk->width() - pWindow->width()) / 2,
         (pDesk->height() - pWindow->height()) / 2);
#endif
    //*/
    //*
    QScreen *pScreen = QGuiApplication::primaryScreen();
    LOG_MODEL_DEBUG("CTool", "availableGeometry:%d,%d; geometry:%d,%d",
                    pScreen->availableGeometry().width(),
                    pScreen->availableGeometry().height(),
                    pScreen->geometry().width(),
                    pScreen->geometry().height());
#ifdef MOBILE
    pWindow->setGeometry(pScreen->availableGeometry());
#else
    if(!(pScreen->availableGeometry().width() > pWindow->width()
            && pScreen->availableGeometry().height() > pWindow->height()))
    {
        pWindow->setGeometry(0, 0, pScreen->availableGeometry().width() - 80,
                             pScreen->availableGeometry().height() - 80);
    }
    pWindow->move((pScreen->availableGeometry().width() - pWindow->width()) >> 1,
                  (pScreen->availableGeometry().height() - pWindow->height()) >> 1);
#endif
    //*/
    return 0;
}

QString CTool::FileDialog(QWidget *pParent, const QString &szDir,
                          const QString &szFilter, const QString &szTilte, const QFileDialog::AcceptMode mode)
{
    QString szFile;
    QFileDialog dlg(pParent, szTilte, szDir, szFilter);
    dlg.setAcceptMode(mode);
#ifdef MOBILE
    dlg.setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    CTool::SetWindowsGeometry(&dlg);
    QStringList fileNames;
    if(dlg.exec())
        fileNames = dlg.selectedFiles();
    else
        return szFile;
    if(fileNames.isEmpty())
        return szFile;
    szFile = *fileNames.begin();

    return szFile;
}

std::string CTool::DoubleToString(double d)
{
    //Need #include <sstream>
    std::string str;
    std::stringstream ss;
    ss << d;
    ss >> str;
    return str;
}

QByteArray CTool::GetFileMd5Sum(QString filePath)
{
    QFile localFile(filePath);
    
    if (!localFile.open(QFile::ReadOnly))
    {
        LOG_MODEL_ERROR("CTool", "file open error.");
        return 0;
    }
    
    QCryptographicHash ch(QCryptographicHash::Md5);
    
    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;
    
    totalBytes = localFile.size();
    bytesToWrite = totalBytes;
    
    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }
        
        if(bytesWritten == totalBytes)
        {
            break;
        }
    }
    
    localFile.close();
    QByteArray md5 = ch.result();
    return md5;
}

QString CTool::GetFileMd5SumString(QString filePath)
{
    return GetFileMd5Sum(filePath).toHex();
}

QPixmap CTool::ComposePixmap(const QString &szFile1, const QString &szFile2)
{
    QPixmap src1(szFile1);
    QPixmap src2(szFile2);
    ComposePixmap(src1, src2);
    return src1;
}

int CTool::ComposePixmap(QPixmap &src1, const QPixmap &src2)
{
    QPainter painter(&src1);
    //painter.setCompositionMode(QPainter::CompositionMode_Xor);
    painter.drawPixmap(0, 0,  src2);
    return 0;
}

int CTool::ComposeAvatarStatus(QPixmap &src1, const QPixmap &src2)
{
    QPainter painter(&src1);
    painter.drawPixmap((src1.width() * 3) >> 2,  (src1.height() * 3) >> 2,
                       src1.width() >> 2, src1.height() >> 2,  src2);
    return 0;
}

uchar RGBtoGRAY(uchar r, uchar g, uchar b)  
{  
    return (uchar)((((qint32)((r << 5) + (r << 2) + (r << 1)))
                    + (qint32)((g << 6) + (g << 3) + (g << 1) + g) 
                    + (qint32)((b << 4) - b)) >> 7);  
}  
//转为灰度图  
QImage CTool::ConvertToGray(QImage image)
{
    QSize size = image.size();
    QImage grayImage = QImage(size, QImage::Format_Indexed8);
    int width = size.width();
    int height = size.height();
    uchar * rgbImageData = image.bits();
    uchar * grayImageData = grayImage.bits();
    if(image.allGray())
    {
        grayImage = QImage(image);
        return grayImage;
    }
    //若width不是4的倍数，会自动添加字节，使之对齐到4的倍数  
    int realWidth1=image.bytesPerLine();
    int realWidth2=grayImage.bytesPerLine();
    uchar * backup1 = rgbImageData;
    uchar * backup2 = grayImageData;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            uchar R = *rgbImageData;
            uchar G = *(rgbImageData + 1);
            uchar B = *(rgbImageData + 2);
            //*grayImageData = (uchar)(0.29900*R + 0.58700*G + 0.11400*B);
            *grayImageData = RGBtoGRAY(R, G, B); 
            rgbImageData += 3;
            grayImageData++;
        }
        rgbImageData = backup1 + realWidth1 * i;
        grayImageData = backup2 + realWidth2 * i;
    }
    
    return grayImage;
    //下面算法可以达到效果，但性能很差  
    //    outGrayImage = rgbImage;
    //    for(int x = 0; x < rgbImage.width(); x++)
    //        for(int y = 0; y < rgbImage.width(); y++)
    //        {
    //            QRgb pix = rgbImage.pixel(x, y);
    //            int gray = qGray(pix);
    //            QRgb grayPixel = qRgb(gray, gray, gray);
    //            outGrayImage.setPixel(x, y, grayPixel);
    //        }
    //return true;
}

bool CTool::EnableWake(bool bWake)
{
    static bool bSet = false;
#ifdef ANDROID
    jboolean bPara = bWake;
    jboolean bRet = QAndroidJniObject::callStaticMethod<jboolean>(
                "org/KangLinStudio/RabbitIm/RabbitImActivity",
                "EnableWake",
                "(Z)Z",
                bPara);
    return bRet;
#elif WINDOWS
    if(bWake)
    {
        BOOL bScrActive;
        SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,0,&bScrActive,0);
        if(bScrActive)
        {
            bSet = true;
            SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, 0);
        }
    } else if(bSet) {
        SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, 0);
        bSet = false;
    }
    return true;
#endif
    return false;
}
