#ifndef CCAMERA_H
#define CCAMERA_H

#include <QObject>

class CLbsCamera : public QObject
{
    Q_OBJECT
public:
    explicit CLbsCamera(QObject *parent = 0);
    
    static CLbsCamera* Instance();

public slots:
    virtual void slotOpen(const QString &szSaveFile = QString()) = 0; //打开摄像机  

signals:
    void sigPhotograph(const QString& szSaveFile);
    
public slots:
};

#endif // CCAMERA_H
