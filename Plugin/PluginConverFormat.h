#ifndef CCONVERFORMAT_H_KL_2020_02
#define CCONVERFORMAT_H_KL_2020_02

#pragma once

#include <QObject>
#include <QVideoFrame>
#include "Global/GlobalDir.h"

/**
 * @brief The CConverFormat class. It is conver format of image,
 *        implemented by plugin
 */
class RABBITIM_SHARED_LIBRARY CConverFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    
public:
    explicit CConverFormat(QObject *parent = nullptr);

    virtual QImage onConverFormatToRGB888(const QVideoFrame &frame) = 0;

    virtual int Initialize();
    virtual int Clean();

    virtual QString getName();
};

#define rabbitim_plugin_conver_format_iid "KangLinStudio.Rabbit.Im.Plugs.ImageTool.ConverFormat"
Q_DECLARE_INTERFACE(CConverFormat, rabbitim_plugin_conver_format_iid)
#endif // CCONVERFORMAT_H_KL_2020_02
