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

Q_DECLARE_INTERFACE(CConverFormat, "KangLinStudio.Rabbit.Im.Plugs.ImageTool.ConverFormat")
#endif // CCONVERFORMAT_H_KL_2020_02
