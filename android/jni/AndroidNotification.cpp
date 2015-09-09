/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "AndroidNotification.h"

#include <QtAndroidExtras/QAndroidJniObject>
#include "Global/Global.h"
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
                "org/KangLinStudio/RabbitIm/NotificationClient", "CancelAll");
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
