/**
  @author: Kang Lin<kl222@126.com>
  */

#include "FrmDisplay.h"

#include <QDebug>
#include <QPainter>

CFrmDisplay::CFrmDisplay(QWidget *parent) :
    QWidget(parent),
    m_bAspectRation(true)
{}

CFrmDisplay::~CFrmDisplay()
{}

int CFrmDisplay::slotSetAspectRatio(bool bAspectRation)
{
    m_bAspectRation = bAspectRation;
    return 0;
}

void CFrmDisplay::slotDisplay(const QImage &image)
{
    m_Image = image;
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(this->isHidden())
        return;
    if(m_Image.isNull()) return;
    
    QPainter painter(this);
    // 设置平滑模式
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRectF dstRect = rect();
    if(m_bAspectRation)
    {
        //m_Image = m_Image.scaled(rect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); 
        qreal newW = dstRect.width();
        qreal newH = dstRect.height();
        qreal newT = 0;
        qreal newL = 0;
      
        qreal rateW = static_cast<qreal>(rect().width())
                / static_cast<qreal>(m_Image.width());
        qreal rateH = static_cast<qreal>(rect().height())
                / static_cast<qreal>(m_Image.height());
        if(rateW < rateH)
        {
            newW = m_Image.width() * rateW;
            newH = m_Image.height() * rateW;
            newT = (static_cast<qreal>(rect().height()) - newH)
                    / static_cast<qreal>(2);
        } else if(rateW > rateH) {
            newW = m_Image.width() * rateH;
            newH = m_Image.height() * rateH;
            newL = (static_cast<qreal>(rect().width()) - newW)
                    / static_cast<qreal>(2);
        }
        dstRect = QRectF(newL, newT, newW, newH);
        
    }
    painter.drawImage(dstRect, m_Image);
}
