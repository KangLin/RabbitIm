#include "webrtc/base/thread.h"
#undef emit

#include "WebrtcFilter.h"
#include "WebrtcConductor.h"
#include <QEvent>

CWebrtcFilter::CWebrtcFilter(CWebrtcConductor *pWebrtcConductor, QObject *parent) 
    : QObject(parent)
{
    m_pWebrtcConductor = pWebrtcConductor;
}

bool CWebrtcFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(obj != this)
        return false;
    if(event->type() != QEvent::User)
        return false;
    m_pWebrtcConductor->m_pSignalThread->ProcessMessages(0);
    return true;
}
