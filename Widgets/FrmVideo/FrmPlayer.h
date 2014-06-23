#ifndef FRMPLAYER_H
#define FRMPLAYER_H

#include <QWidget>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "qxmpp/QXmppRtpChannel.h"
#include "FrameProcess.h"

class CFrmPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPlayer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFrmPlayer();

private:
signals:
    //需要转换格式信号，用于内部单独线进行图片格式转换
    void sigConverteToRGB32Frame(const QVideoFrame &frame, const QRect &rect);
    void sigConverteToRGB32Frame(const QXmppVideoFrame &frame, const QRect &rect);

public slots:
    //捕获视频槽
    virtual void slotPresent(const QVideoFrame &frame);
    virtual void slotPresent(const QXmppVideoFrame &frame);

private slots:
    void slotPaint(const QVideoFrame &frame);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QVideoFrame m_VideoFrame;
    CFrameProcess m_FrameProcess;//格式转换实例
};

#endif // FRMPLAYER_H
