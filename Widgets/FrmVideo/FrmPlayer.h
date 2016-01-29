#ifndef FRMPLAYER_H
#define FRMPLAYER_H

#include <QWidget>
#include <Media/Camera/Camera.h>
#include "Widgets/FrmVideo/FrameProcess.h"

class RABBITIM_SHARED_LIBRARY CFrmPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPlayer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFrmPlayer();

public slots:
    void slotPresent(std::shared_ptr<CVideoFrame> frame);
    void slotPresent(const QVideoFrame &frame);
    void slotPresent(const QImage &frame);
private slots:
    void slotPresentRGB32(const QVideoFrame &frame);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void changeEvent(QEvent*);

private:
    std::shared_ptr<CVideoFrame> m_Frame;
    QVideoFrame m_VideoFrame;
    CFrameProcess m_Process;
    QImage m_Image;

#ifdef DEBUG
public:
    int TestCamera();
#endif
};

#endif // FRMPLAYER_H
