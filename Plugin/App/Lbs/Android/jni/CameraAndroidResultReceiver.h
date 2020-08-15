#ifndef CCAMERAANDROIDRESULTRECEIVER_H
#define CCAMERAANDROIDRESULTRECEIVER_H

#include <QAndroidActivityResultReceiver>
#include <QString>
class CCameraAndroid;
class CCameraAndroidResultReceiver : public QAndroidActivityResultReceiver
{
public:
    CCameraAndroidResultReceiver(CCameraAndroid* pCamera);
    int SetSaveFile(const QString &szFile);
    virtual void handleActivityResult(int receiverRequestCode,
                                      int resultCode,
                                      const QAndroidJniObject &data);
    
signals:
    void sigPhotograph(const QString& szFile);
    
private:
    QString m_szFile;
    CCameraAndroid* m_pCamera;
};

#endif // CCAMERAANDROIDRESULTRECEIVER_H
