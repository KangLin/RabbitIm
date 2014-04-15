#ifndef FRMPLAYER_H
#define FRMPLAYER_H

#include "../../Tool.h"
#include <QWidget>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "qxmpp/QXmppRtpChannel.h"

class CFrmPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPlayer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFrmPlayer();

public slots:
    //捕获视频槽
    virtual void present(const QVideoFrame &frame);
    virtual void present(const QXmppVideoFrame &frame);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QXmppVideoFrame *m_pVideoFrame;

    void present(AVPicture &pic);
};

#endif // FRMPLAYER_H
