#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QObject>
#include <QVideoFrame>

class CCaptureFrameProcess : public QObject
{
    Q_OBJECT
public:
    explicit CCaptureFrameProcess(QObject *parent = 0);
    virtual ~CCaptureFrameProcess();

signals:
    //帧被处理完后的信号
    void sigCaptureFrame(const QVideoFrame &frame);

public slots:
    //处理帧的槽
    void slotCaptureFrame(const QVideoFrame &frame);

};

#endif // VIDEOPROCESS_H
