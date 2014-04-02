#ifndef FRMPLAYER_H
#define FRMPLAYER_H

#include <QWidget>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"

class CFrmPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmPlayer(QWidget *parent = 0);
    ~CFrmPlayer();

public slots:
    virtual void present(const QVideoFrame &frame);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QXmppVideoFrame *m_pVideoFrame;
};

#endif // FRMPLAYER_H
