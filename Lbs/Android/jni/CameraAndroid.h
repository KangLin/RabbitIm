#ifndef CCAMERAANDROID_H
#define CCAMERAANDROID_H

#include "../../LbsCamera.h"
#include "CameraAndroidResultReceiver.h"

class CCameraAndroid : public CLbsCamera
{
    Q_OBJECT
    
public:
    CCameraAndroid(QObject *parent = 0);

    enum {
        RequestCodePhotograph = 1 
    };
public slots:
    virtual void slotOpen(const QString &szSaveFile = QString());

private:
    CCameraAndroidResultReceiver m_Receiver;    
};

#endif // CCAMERAANDROID_H
