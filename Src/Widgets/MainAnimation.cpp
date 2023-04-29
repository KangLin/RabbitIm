#include "MainAnimation.h"
#include "Global/Global.h"

CMainAnimation::CMainAnimation(QWidget *parent)
    : QWidget(parent,
              Qt::FramelessWindowHint
              | Qt::X11BypassWindowManagerHint  //这个标志是在x11下有用,查看帮助QWidget::showFullScreen()  
              | Qt::WindowStaysOnTopHint)      

{
}

void CMainAnimation::enterEvent(QEvent* event)
{
    Q_UNUSED(event)
    qDebug("MainWindow::enterEvent");

#ifndef MOBILE
    emit sigShow();
#endif
}

void CMainAnimation::mouseMoveEvent(QMouseEvent *)
{
//    emit sigShow();
}
