#include "CameraAndroidResultReceiver.h"
#include "../../../Global/Log.h"
#include "CameraAndroid.h"

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
        int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    LOG_MODEL_DEBUG("CCameraAndroidResultReceiver",
              "handleActivityResult:receiverRequestCode:%d,resultCode:%d",
              receiverRequestCode, resultCode);
    if(-1 == resultCode
         && CCameraAndroid::RequestCodePhotograph == receiverRequestCode)
    {
        if(m_pCamera)
        {
            LOG_MODEL_DEBUG("CCameraAndroidResultReceiver",
                            "emit sigPhotograph");
            emit m_pCamera->sigPhotograph(m_szFile);
        }
    }
}
