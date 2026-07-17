#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>
#include "CameraAndroid.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QJniObject>
#else
    #include <QtAndroid>
    #include <QAndroidJniEnvironment>
#endif
#include "Global/GlobalDir.h"

static Q_LOGGING_CATEGORY(log, "CCameraAndroid")
#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
        qDebug(log) << "exception occurred"; \
        env->ExceptionClear();\
    }

CCameraAndroid::CCameraAndroid(QObject *parent)
    : CLbsCamera(parent), m_Receiver(this)
{
}

void CCameraAndroid::slotOpen(const QString &szSaveFile)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;

    //construct Intent for IMAGE_CAPTURE
    QJniObject action = QJniObject::fromString(
        "android.media.action.IMAGE_CAPTURE");
    QJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action.object<jstring>());
    QString szFile = szSaveFile;
    if(szFile.isEmpty())
    {
        //setup saved image location
        szFile = CGlobalDir::Instance()->GetDirMotion()
                 + QDir::separator() + "photo";
        QDir d;
        if(!d.exists(szFile))
            d.mkpath(szFile);
        szFile += QDir::separator()
                  + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")
                  + ".jpg";
    }
    QJniObject fileName = QJniObject::fromString(szFile);
    QJniObject savedImageFile(
        "java/io/File",
        "(Ljava/lang/String;)V",
        fileName.object<jstring>());
    CHECK_EXCEPTION()

    QJniObject savedImageUri =
        QJniObject::callStaticObjectMethod(
            "android/net/Uri",
            "fromFile",
            "(Ljava/io/File;)Landroid/net/Uri;",
            savedImageFile.object<jobject>());
    CHECK_EXCEPTION()

    //tell IMAGE_CAPTURE the output location
    QJniObject mediaStoreExtraOutput
        = QJniObject::getStaticObjectField(
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
    QtAndroidPrivate::startActivity(intent, RequestCodePhotograph, &m_Receiver);
    CHECK_EXCEPTION()
#else
    QAndroidJniEnvironment env;

    //construct Intent for IMAGE_CAPTURE
    QAndroidJniObject action = QAndroidJniObject::fromString(
                "android.media.action.IMAGE_CAPTURE");
    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action.object<jstring>());
    QString szFile = szSaveFile;
    if(szFile.isEmpty())
    {
        //setup saved image location
        szFile = CGlobalDir::Instance()->GetDirMotion()
                         + QDir::separator() + "photo";
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
#endif
}
