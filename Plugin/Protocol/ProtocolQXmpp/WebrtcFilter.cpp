#include "webrtc/base/thread.h"
#undef emit

#include "WebrtcFilter.h"
#include "WebrtcConductor.h"
#include <QEvent>

CWebrtcFilter::CWebrtcFilter(QObject *parent) 
    : QObject(parent)
{
}

bool CWebrtcFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(obj != this)
        return false;
    if(event->type() != QEvent::User)
        return false;
    CWebrtcConductor::m_pSignalThread->ProcessMessages(0);
    return true;
}

CWebrtcFilter* CWebrtcFilter::Instance()
{
    static CWebrtcFilter* p = NULL;
    if(NULL == p)
    {
        p = new CWebrtcFilter();
    }
    return p;
}
