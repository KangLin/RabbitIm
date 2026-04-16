#include <QLoggingCategory>
#include "MainAnimation.h"
#include "Global/Global.h"

static Q_LOGGING_CATEGORY(log, "Animation")
CMainAnimation::CMainAnimation(QWidget *parent)
    : QWidget(parent,
              Qt::FramelessWindowHint
              | Qt::X11BypassWindowManagerHint  //这个标志是在x11下有用,查看帮助QWidget::showFullScreen()
              | Qt::WindowStaysOnTopHint)      

{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void CMainAnimation::enterEvent(QEnterEvent *event)
#else
void CMainAnimation::enterEvent(QEvent* event)
#endif
{
    Q_UNUSED(event)
    qDebug(log) << Q_FUNC_INFO;

#ifndef MOBILE
    emit sigShow();
#endif
}

void CMainAnimation::mouseMoveEvent(QMouseEvent *)
{
//    emit sigShow();
}
