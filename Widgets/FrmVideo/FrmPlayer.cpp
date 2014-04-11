
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

//从摄像头捕获的帧
void CFrmPlayer::present(const QVideoFrame &frame)
{
    QVideoFrame f(frame);

    //QVideoFrame使用bits前一定要先map，bits才会生效
    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return;

    do
    {
        QRect rect = this->rect();
        //图片格式转换
        AVPicture pic;
        int nRet = CTool::ConvertFormat(f, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
        if(nRet)
            break;

        present(pic);

        avpicture_free(&pic);
    }while(0);

    f.unmap();

    this->update();

    return;
}

//从网络上接收的帧
void CFrmPlayer::present(const QXmppVideoFrame &frame)
{
    QRect rect = this->rect();
    //图片格式转换
    AVPicture pic;
    int nRet = CTool::ConvertFormat(frame, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
    if(nRet)
        return;

    present(pic);
    avpicture_free(&pic);


    this->update();

    return;
}

void CFrmPlayer::present(AVPicture &pic)
{
    QRect rect = this->rect();
    int size = avpicture_get_size(AV_PIX_FMT_RGB32, rect.width(), rect.height());
    if(NULL == m_pVideoFrame)
    {
        QSize frameSize(rect.width(), rect.height());
        m_pVideoFrame = new QXmppVideoFrame(size,
                                            frameSize,
                                            size / rect.height(),
                                            QXmppVideoFrame::Format_RGB32);
    }

    if(m_pVideoFrame)
    {
        avpicture_layout(&pic, AV_PIX_FMT_RGB32,
                         rect.width(), rect.height(),
                         m_pVideoFrame->bits(), size);
    }

}
