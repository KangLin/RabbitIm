#include "MainAnimation.h"
#include "Global/Global.h"

CMainAnimation::CMainAnimation(QWidget *parent)
    : QWidget(parent,
              Qt::FramelessWindowHint
              | Qt::WindowStaysOnTopHint)      

{
}

void CMainAnimation::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    LOG_MODEL_DEBUG("CMainAnimation", "MainWindow::enterEvent");

#ifndef MOBILE
    emit sigShow();
#endif
}

void CMainAnimation::mouseMoveEvent(QMouseEvent *)
{
//    emit sigShow();
}
