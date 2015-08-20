#include "CameraAndroid.h"
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include "../../../Global/Log.h"

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
        LOG_MODEL_ERROR("CCameraAndroid", "exception occured"); \
        env->ExceptionClear();\
    }

CCameraAndroid::CCameraAndroid(QObject *parent)
    : CLbsCamera(parent), m_Receiver(this)
{
}

void CCameraAndroid::slotOpen(const QString &szSaveFile)
{
    QAndroidJniEnvironment env;

    //constuct Intent for IMAGE_CAPTURE
    QAndroidJniObject action = QAndroidJniObject::fromString(
                "android.media.action.IMAGE_CAPTURE");
    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action.object<jstring>());
    QString szFile = szSaveFile;
    if(szFile.isEmpty())
    {
        //setup saved image location
        //TODO:修改文件  
        szFile =
                QStandardPaths::writableLocation(
                    QStandardPaths::DocumentsLocation)
                + QDir::separator() + "Rabbit/photo";
        QDir d;
        if(!d.exists(szFile))
            d.mkpath(szFile);
        szFile += QDir::separator()
                + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")
                + ".jpg";
    }
    QAndroidJniObject fileName = QAndroidJniObject::fromString(szFile);
    QAndroidJniObject savedImageFile(
                "java/io/File",
                "(Ljava/lang/String;)V",
                fileName.object<jstring>());
    CHECK_EXCEPTION()

    QAndroidJniObject savedImageUri =
            QAndroidJniObject::callStaticObjectMethod(
                "android/net/Uri",
                "fromFile",
                "(Ljava/io/File;)Landroid/net/Uri;",
                savedImageFile.object<jobject>());
    CHECK_EXCEPTION()

    //tell IMAGE_CAPTURE the output location
    QAndroidJniObject mediaStoreExtraOutput
            = QAndroidJniObject::getStaticObjectField(
                "android/provider/MediaStore",
                "EXTRA_OUTPUT",
                "Ljava/lang/String;");
    CHECK_EXCEPTION()

    intent.callObjectMethod(
                "putExtra",
                "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
                mediaStoreExtraOutput.object<jstring>(),
                savedImageUri.object<jobject>());

    m_Receiver.SetSaveFile(szFile);
    QtAndroid::startActivity(intent, RequestCodePhotograph, &m_Receiver);
    CHECK_EXCEPTION()
}
