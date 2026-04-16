#ifndef CIMAGETOOLOPENCV_H
#define CIMAGETOOLOPENCV_H

#include "PluginConvertFormat.h"

class CConvertFormatOpenCV : public CPluginConvertFormat
{
    Q_OBJECT
public:
    explicit CConvertFormatOpenCV(QObject *parent = nullptr);

    Q_INTERFACES(CPluginConvertFormat)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID rabbitim_plugin_convert_format_iid)
#endif

    virtual QImage onConvertFormatToRGB888(const QVideoFrame &frame) override;
};

#endif // CIMAGETOOLOPENCV_H
