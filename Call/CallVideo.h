#ifndef CALLVIDEO_H
#define CALLVIDEO_H

#include "CallObject.h"
#include <QAudioInput>
#include <QAudioOutput>

class CCallVideo : public CCallObject
{
    Q_OBJECT
public:
    explicit CCallVideo(QObject *parent = 0);

};

#endif // CALLVIDEO_H
