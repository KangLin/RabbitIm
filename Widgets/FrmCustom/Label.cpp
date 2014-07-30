#include "Label.h"
#include <QMouseEvent>

CLabel::CLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
}

CLabel::CLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f)
{
}

void CLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        emit clicked();
}
