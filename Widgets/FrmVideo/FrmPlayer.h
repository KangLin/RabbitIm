#ifndef FRMPLAYER_H
#define FRMPLAYER_H

#include <QWidget>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "qxmpp/QXmppRtpChannel.h"

class CFrmPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPlayer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~CFrmPlayer();

public slots:
    //捕获视频槽
    virtual void present(const QVideoFrame &frame);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QXmppVideoFrame *m_pVideoFrame;
};

#endif // FRMPLAYER_H
