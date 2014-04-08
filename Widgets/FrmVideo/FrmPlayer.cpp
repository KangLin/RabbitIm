#include "../../Tool.h"
#include "FrmPlayer.h"
#include <QPainter>
#include <QImage>

CFrmPlayer::CFrmPlayer(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f)
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
    QVideoFrame f(frame);

    //QVideoFrame使用bits前一定要先map，bits才会生效
    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return;

    do
    {
        //图片格式转换
        AVPicture pic;
        int nRet = CTool::ConvertFormat(f, &pic, AV_PIX_FMT_RGB32);
        if(nRet)
            break;

        int size = avpicture_get_size(AV_PIX_FMT_RGB32, f.width(), f.height());
        if(NULL == m_pVideoFrame)
        {
            m_pVideoFrame = new QXmppVideoFrame(size,
                                                f.size(),
                                                size / f.height(),
                                                QXmppVideoFrame::Format_RGB32);
        }

        if(m_pVideoFrame)
        {
            avpicture_layout(&pic, AV_PIX_FMT_RGB32, f.width(), f.height(), m_pVideoFrame->bits(), size);
        }

        avpicture_free(&pic);
    }while(0);

    f.unmap();

    this->update();

    return;
}
