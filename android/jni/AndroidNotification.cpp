#include "AndroidNotification.h"

#include <QtAndroidExtras/QAndroidJniObject>
#include "MainWindow.h"

CAndroidNotification::CAndroidNotification(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(sigNotificationChanged(QString,QString)),
            this, SLOT(slotUpdateAndroidNotification(QString,QString)));
}

CAndroidNotification::~CAndroidNotification()
{
    QAndroidJniObject::callStaticMethod<void>(
                "org/KangLinStudio/RabbitIm/RabbitImActivity", "CancelAll");
}

void CAndroidNotification::setNotification(
        const QString &szMessage, const QString &szTitle)
{
    QString s = szTitle;
    if(szTitle.isEmpty())
        s = tr("RabbitIm");
    emit sigNotificationChanged(szMessage, s);
}

void CAndroidNotification::slotUpdateAndroidNotification(
        const QString &szMessage, const QString &szTitle)
{
    QAndroidJniObject javaNotification =
            QAndroidJniObject::fromString(szMessage);
    QAndroidJniObject javaTitle = QAndroidJniObject::fromString(szTitle);
    QAndroidJniObject::callStaticMethod<void>(
                "org/KangLinStudio/RabbitIm/RabbitImActivity",
                "notify",
                "(Ljava/lang/String;Ljava/lang/String;)V",
                javaNotification.object<jstring>(),
                javaTitle.object<jstring>());
}

static JavaVM *javaVM = 0;
jclass RabbitImActivityClass;
static void OnChickNotification(JNIEnv *env, jobject thiz, jstring key)
{
    LOG_MODEL_DEBUG("CAndroidNotification", "OnChickNotification");
    QString szKey;
    if(GET_MAINWINDOW)
        emit GET_MAINWINDOW->m_AndroidNotify.sigOnChick();
}

#define FIND_AND_CHECK_CLASS(CLASS_NAME) \
clazz = env->FindClass(CLASS_NAME); \
if (!clazz) { \
    LOG_MODEL_ERROR("CAndroidNotification", "don't find class name:%s", CLASS_NAME); \
    return JNI_FALSE; \
}

#define GET_AND_CHECK_STATIC_METHOD(VAR, CLASS, METHOD_NAME, METHOD_SIGNATURE) \
VAR = env->GetStaticMethodID(CLASS, METHOD_NAME, METHOD_SIGNATURE); \
if (!VAR) { \
   LOG_MODEL_ERROR("CAndroidNotification", "don't find method:%s;para:%s", METHOD_NAME, METHOD_SIGNATURE); \
    return JNI_FALSE; \
}

static JNINativeMethod methods[] = {
    {"OnChickNotification", "(Ljava/lang/String;)V", (void *)OnChickNotification}
};

static bool registerNatives(JNIEnv *env)
{
    jclass clazz;
    FIND_AND_CHECK_CLASS("org/KangLinStudio/RabbitIm/RabbitImActivity");
    RabbitImActivityClass = static_cast<jclass>(env->NewGlobalRef(clazz));

    if (env->RegisterNatives(RabbitImActivityClass, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOG_MODEL_ERROR("CAndroidNotification", "RegisterNatives failed");
        return JNI_FALSE;
    }

    return true;
}

Q_DECL_EXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/)
{
    typedef union {
        JNIEnv *nativeEnvironment;
        void *venv;
    } UnionJNIEnvToVoid;

    LOG_MODEL_DEBUG("CAndroidNotification", "JNI_OnLoad start");
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    javaVM = 0;

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOG_MODEL_ERROR("CAndroidNotification", "GetEnv failed");
        return -1;
    }
    JNIEnv *env = uenv.nativeEnvironment;
    if (!registerNatives(env)) {
        LOG_MODEL_ERROR("CAndroidNotification", "registerNatives failed");
        return -1;
    }

    javaVM = vm;
    return JNI_VERSION_1_4;
}
