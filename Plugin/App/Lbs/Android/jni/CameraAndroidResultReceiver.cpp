#include <QLoggingCategory>

#include "CameraAndroidResultReceiver.h"

#include "CameraAndroid.h"

static Q_LOGGING_CATEGORY(log, "CCameraAndroidResultReceiver")
CCameraAndroidResultReceiver::CCameraAndroidResultReceiver(CCameraAndroid* pCamera)
{  
    m_pCamera = pCamera;
}

int CCameraAndroidResultReceiver::SetSaveFile(const QString &szFile)
{
    m_szFile = szFile;
    return 0;
}

void CCameraAndroidResultReceiver::handleActivityResult(
        int receiverRequestCode, int resultCode,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QJniObject &data
#else
    const QAndroidJniObject &data
#endif
    )
{
    qDebug(log, "handleActivityResult:receiverRequestCode:%d,resultCode:%d",
              receiverRequestCode, resultCode);
    if(-1 == resultCode
         && CCameraAndroid::RequestCodePhotograph == receiverRequestCode)
    {
        if(m_pCamera)
        {
            qDebug(log) << "emit sigPhotograph";
            emit m_pCamera->sigPhotograph(m_szFile);
        }
    }
}
