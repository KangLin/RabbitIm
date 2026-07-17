#pragma once

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/private/qandroidextras_p.h>
#else
    #include <QAndroidActivityResultReceiver>
#endif

#include <QString>
class CCameraAndroid;
class CCameraAndroidResultReceiver : public QAndroidActivityResultReceiver
{
public:
    CCameraAndroidResultReceiver(CCameraAndroid* pCamera);
    int SetSaveFile(const QString &szFile);
    virtual void handleActivityResult(int receiverRequestCode,
                                      int resultCode,
                                      #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                          const QJniObject &data
                                      #else
                                          const QAndroidJniObject &data
                                      #endif
                                      );
    
signals:
    void sigPhotograph(const QString& szFile);
    
private:
    QString m_szFile;
    CCameraAndroid* m_pCamera;
};

