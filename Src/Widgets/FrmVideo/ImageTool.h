/**
  @brief: 提供一些图像处理工具函数
  @author: Kang Lin<kl222@126.com>
  */

#ifndef IMAGE_TOOL_H_KL_2020_01
#define IMAGE_TOOL_H_KL_2020_01

#pragma once
//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//#pragma execution_character_set("utf-8")
//#endif

#include <QObject>
#include <memory>
#include <string>
#include <QWidget>
#include <QVideoFrame>
#include <QFileDialog>
#include <QMetaClassInfo>

#include "Plugin/PluginConverFormat.h"

/**
 * @brief 图形工具，提供一些图形格式转换功能  
 * @ingroup RABBITIM_TOOL
 */
class RABBITIM_SHARED_LIBRARY CImageTool : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    
public:
    explicit CImageTool(QObject *parent = nullptr);
    static CImageTool* Instance();

public:
    QImage ConverFormatToRGB888(const QVideoFrame &frame);

private:
    QList<CPluginConverFormat*> m_ConverFormat;
    
    int FindPlugins(QDir dir, QStringList filters);
    static void YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height);
    
#if HAVE_LIBYUV
    static QImage LibyuvConverFormatToRGB888(const QVideoFrame &frame);
#endif

};

#endif // IMAGE_TOOL_H_KL_2020_01
