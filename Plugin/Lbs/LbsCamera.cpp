#include "LbsCamera.h"
#ifdef ANDROID
    #include "Android/jni/CameraAndroid.h"
#endif

CLbsCamera::CLbsCamera(QObject *parent) : QObject(parent)
{   
}

CLbsCamera* CLbsCamera::Instance()
{
    static CLbsCamera* p = NULL;
    if(NULL == p)
    {
#ifdef ANDROID
        p = new CCameraAndroid();
#endif
    }
    return p;
}

