
#pragma once

#include <QObject>
#include <QVideoFrame>
#include "Global/GlobalDir.h"

/**
 * @brief The CConvertFormat class. It is convert format of image,
 *        implemented by plugin
 */
class RABBITIM_SHARED_LIBRARY CPluginConvertFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(int Priority READ getPriority)
    
public:
    explicit CPluginConvertFormat(QObject *parent = nullptr);

    virtual QImage onConvertFormatToRGB888(const QVideoFrame &frame) = 0;

    virtual int Initialize();
    virtual int Clean();

    virtual QString getName();
    virtual int getPriority();
};

#define rabbitim_plugin_convert_format_iid "KangLinStudio.Rabbit.Im.Plugs.ImageTool.ConvertFormat"
Q_DECLARE_INTERFACE(CPluginConvertFormat, rabbitim_plugin_convert_format_iid)
