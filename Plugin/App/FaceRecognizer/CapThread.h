#ifndef CAPTHREAD_H
#define CAPTHREAD_H

#include <QObject>
#include <QThread> 
#include <opencv2\opencv.hpp>

class CCapThread : public QThread
{
    Q_OBJECT
public:
    explicit CCapThread(QObject *parent = 0);
    virtual ~CCapThread();
    
    virtual void run();
    void Stop(bool bStop = true);
    
signals:
    void sigCaptured(cv::Mat frame);
    
private:
    bool m_bStop;
};

#endif // CAPTHREAD_H
