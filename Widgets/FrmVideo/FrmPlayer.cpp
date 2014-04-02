#include "FrmPlayer.h"
#include <QPainter>
#include <QImage>

CFrmPlayer::CFrmPlayer(QWidget *parent) :
    QWidget(parent)
{
    m_pVideoFrame = NULL;
}

CFrmPlayer::~CFrmPlayer()
{
    if(m_pVideoFrame)
        delete m_pVideoFrame;
}

void CFrmPlayer::paintEvent(QPaintEvent *)
{
    if(!m_pVideoFrame)
        return;

    QPainter painter(this);
    QImage image(m_pVideoFrame->bits(),
                 m_pVideoFrame->width(),
                 m_pVideoFrame->height(),
                 m_pVideoFrame->bytesPerLine(),
                 QImage::Format_RGB32);
    painter.drawImage(this->rect(), image);
}

void CFrmPlayer::present(const QVideoFrame &frame)
{
    if(QVideoFrame::Format_RGB32 != frame.pixelFormat())
    {
        qDebug("CFrmPlayer::ShowFrame:frame pixel format is not Format_RGB32");
        return;
    }

    QVideoFrame f(frame);

    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return;

    if(NULL == m_pVideoFrame)
    {
        m_pVideoFrame = new QXmppVideoFrame(f.mappedBytes(),
                                            f.size(),
                                            f.bytesPerLine(),
                                            QXmppVideoFrame::Format_RGB32);
    }

    if(m_pVideoFrame)
    {
        memcpy(m_pVideoFrame->bits(), f.bits(), m_pVideoFrame->mappedBytes());
    }

    f.unmap();

    this->update();

    return;
}
