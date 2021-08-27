#include "ImageTool.h"
#if HAVE_LIBYUV
    #include "libyuv.h"
#endif
#include "RabbitCommonDir.h"
#include "RabbitCommonLog.h"
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
#include <QPluginLoader>

CImageTool::CImageTool(QObject *parent) : QObject(parent), m_pConverFormat(nullptr)
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        m_pConverFormat = qobject_cast<CConverFormat*>(plugin);
        if(m_pConverFormat)
            break;
    }

    QString szPath = RabbitCommon::CDir::Instance()->GetDirPlugins();
#if !defined (Q_OS_ANDROID)
    szPath = szPath + QDir::separator() + "ConverFormat";
#endif
    QStringList filters;
#if defined (Q_OS_WINDOWS)
        filters << "*.dll";
#else
        filters << "*.so";
#endif
    FindPlugins(szPath, filters);
}

CImageTool* CImageTool::Instance()
{
    static CImageTool* p = nullptr;
    if(!p)
        p = new CImageTool();
    return p;
}

// 图像格式介绍：
//    https://blog.csdn.net/byhook/article/details/84037338
//    https://blog.csdn.net/cgwang_1580/article/details/79595958
QImage CImageTool::ConverFormatToRGB888(const QVideoFrame &frame)
{
    switch(frame.pixelFormat())
    {
    case QVideoFrame::Format_YUV420P:
    case QVideoFrame::Format_YV12:
    case QVideoFrame::Format_NV21:
    case QVideoFrame::Format_NV12:
    case QVideoFrame::Format_YUYV:
    case QVideoFrame::Format_UYVY:
    {
        if(m_pConverFormat)
        {
            QImage image = m_pConverFormat->onConverFormatToRGB888(frame);
            if(!image.isNull()) return image;
        }

#ifdef HAVE_LIBYUV
        return  LibyuvConverFormatToRGB888(frame);
#endif
        if(QVideoFrame::Format_YUV420P != frame.pixelFormat())
        {
            LOG_MODEL_WARNING("CImageTool", "Please use one of opencv, ffmpeg, libyuv");
        }
        break;
    }
    default:
        break;
    }

    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;
    do{
        QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                    videoFrame.pixelFormat());
        if(QImage::Format_Invalid != f)
        {
            img = QImage(videoFrame.bits(),
                         videoFrame.width(),
                         videoFrame.height(),
                         videoFrame.bytesPerLine(),
                         f).convertToFormat(QImage::Format_RGB888);
        } else {
            switch(videoFrame.pixelFormat())
            {
            case QVideoFrame::Format_Jpeg:
                {
                    img.loadFromData(videoFrame.bits(),
                                     videoFrame.mappedBytes(),
                                     "JPEG");
                }
                break;
            case QVideoFrame::Format_YUV420P:
                YUV420_2_RGB(videoFrame.bits(), img.bits(),
                             videoFrame.width(), videoFrame.height());
                break;
            default:
                LOG_MODEL_ERROR("CImageTool",  "Don't implement conver format: %d",
                                videoFrame.pixelFormat());
            }
        }
    }while(0);
    videoFrame.unmap();
    return img;
}

#if HAVE_LIBYUV
QImage CImageTool::LibyuvConverFormatToRGB888(const QVideoFrame &frame)
{
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;
    
    PERFORMANCE(LibyuvConverFormatToRGB888)
    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        switch(videoFrame.pixelFormat())
        {
        case QVideoFrame::Format_YUV420P:
        {
            libyuv::I420ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width() >> 1, // / 2,
                                videoFrame.bits() +  videoFrame.width() * videoFrame.height() * 5 / 4,
                                videoFrame.width() >> 1, // / 2
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            break;
        case QVideoFrame::Format_YV12:
            libyuv::I420ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() +  videoFrame.width() * videoFrame.height() * 5 / 4,
                                videoFrame.width() >> 1, // / 2
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width() >> 1, // / 2,
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
            break;
        case QVideoFrame::Format_NV21:
        {
            libyuv::NV21ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width(),
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            break;
        case QVideoFrame::Format_NV12:
        {
            libyuv::NV12ToRGB24(videoFrame.bits(),
                                videoFrame.width(),
                                videoFrame.bits() + videoFrame.width() * videoFrame.height(),
                                videoFrame.width(),
                                img.bits(),
                                videoFrame.width() * 3,
                                videoFrame.width(),
                                videoFrame.height());
        }
            break;
        case QVideoFrame::Format_YUYV:
        {
            img = QImage(videoFrame.width(),
                         videoFrame.height(),
                         QImage::Format_ARGB32);
            libyuv::YUY2ToARGB(videoFrame.bits(),
                                videoFrame.width() << 1, // * 2
                                img.bits(),
                                videoFrame.width() << 2, // * 4
                                videoFrame.width(),
                                videoFrame.height());
            img = img.convertToFormat(QImage::Format_RGB888);
        }
            break;
        case QVideoFrame::Format_UYVY:
        {
            img = QImage(videoFrame.width(),
                         videoFrame.height(),
                         QImage::Format_ARGB32);
            libyuv::UYVYToARGB(videoFrame.bits(),
                                videoFrame.width() << 1, // * 2
                                img.bits(),
                                videoFrame.width() << 2, // * 4
                                videoFrame.width(),
                                videoFrame.height());
            img = img.convertToFormat(QImage::Format_RGB888);
        }
            break;
        default:
            LOG_MODEL_WARNING("CImageTool",  "LibyuvConverFormatToRGB888 Don't implement conver format: %d",
                            videoFrame.pixelFormat());
        }
        
    }while(0);
    videoFrame.unmap();
    PERFORMANCE_ADD_TIME(LibyuvConverFormatToRGB888, "LibyuvConverFormatToRGB888")
    return img;
}
#endif

//参见： https://blog.csdn.net/junzia/article/details/76315120
void CImageTool::YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height)
{
	//找到Y、U、V在内存中的首地址  
	unsigned char* pY = pYUV;
	unsigned char* pU = pYUV + height * width;
	unsigned char* pV = pU + (height * width / 4);

	unsigned char* pBGR = nullptr;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char Y = 0;
	unsigned char U = 0;
	unsigned char V = 0;
	double temp = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//找到相应的RGB首地址  
			pBGR = pRGB + i * width * 3 + j * 3;

			//取Y、U、V的数据值  
			Y = *(pY + i*width + j);
			U = *pU;
			V = *pV;

			//yuv转rgb公式  
			//yuv转rgb公式  
			temp = Y + ((1.773) * (U - 128));
			B = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y - (0.344) * (U - 128) - (0.714) * (V - 128));
			G = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y + (1.403)*(V - 128));
			R = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			//将转化后的rgb保存在rgb内存中，注意放入的顺序b是最低位  
			*pBGR = B;
			*(pBGR + 1) = G;
			*(pBGR + 2) = R;
  
			if (j % 2 != 0)
			{
				*pU++;
				*pV++;
			}
 
		}
		if (i % 2 == 0)
		{
			pU = pU - width / 2;
			pV = pV - width / 2;
		}
	}
}

int CImageTool::FindPlugins(QDir dir, QStringList filters)
{
    QString szPath = dir.path();
    QString fileName;
    if(filters.isEmpty())
    {
#if defined (Q_OS_WINDOWS)
        filters << "*.dll";
#else
        filters << "*.so";
#endif
    }

    //This method is invalid
    //QCoreApplication::addLibraryPath(QDir::cleanPath(dir.absolutePath()));

    QDir::setCurrent(QDir::cleanPath(QDir::cleanPath(dir.absolutePath())));
    
    // This method is valid
//#if defined(Q_OS_WINDOWS)
//        QString szPath = QString::fromLocal8Bit(qgetenv("PATH"));
//        szPath += ";";
//        szPath += QDir::cleanPath(QDir::cleanPath(dir.absolutePath()));
//        qputenv("PATH", szPath.toLatin1());
//#endif
    
    QStringList files = dir.entryList(filters, QDir::Files | QDir::CaseSensitive);
    foreach (fileName, files) {
        //LOG_MODEL_INFO("CImageTool", "file name:%s", fileName.toStdString().c_str());
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            m_pConverFormat = qobject_cast<CConverFormat*>(plugin);
            if(m_pConverFormat)
            {
                return 0;
            }
        }else{
            LOG_MODEL_ERROR("CImageTool", "load plugin error:%s",
                            loader.errorString().toStdString().c_str());
        }
    }

    foreach (fileName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir = dir;
        if(pluginDir.cd(fileName))
            FindPlugins(pluginDir, filters);
    }

    return 0;
}
