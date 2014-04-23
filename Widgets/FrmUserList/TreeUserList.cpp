#include "TreeUserList.h"
#include <QDebug>

CTreeUserList::CTreeUserList(QWidget *parent) :
    QTreeView(parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

void CTreeUserList::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "CTreeUserList::mousePressEvent";
    m_MousePressTime = QTime::currentTime();
    QTreeView::mousePressEvent(event);
}

void CTreeUserList::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "CTreeUserList::mouseReleaseEvent";
#ifdef ANDROID
    QTime cur = QTime::currentTime();
    int sec = m_MousePressTime.secsTo(cur);
    qDebug() << "m_MousePressTime:" << m_MousePressTime.toString("hh:mm:ss.zzz")
                << ";currentTime:" << cur.toString("hh:mm:ss.zzz")
                   << ";sect:" << sec;

    if(sec >= 2)
    {
        emit customContextMenuRequested(QCursor::pos());
        return;
    }
#endif
    QTreeView::mouseReleaseEvent(event);
}

void CTreeUserList::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "CTreeUserList::mouseReleaseEvent";
    QTreeView::contextMenuEvent(event);
}
