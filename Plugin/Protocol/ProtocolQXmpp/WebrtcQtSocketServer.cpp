
#include "WebrtcQtSocketServer.h"
#include "WebrtcConductor.h"
#include <QApplication>
#include "WebrtcFilter.h"

CWebrtcQtSocketServer::CWebrtcQtSocketServer()
{
}

bool CWebrtcQtSocketServer::Wait(int cms, bool process_io)
{
    return true;
}

void CWebrtcQtSocketServer::WakeUp()
{
    QEvent *pEvent = new QEvent(QEvent::User);
    qApp->postEvent(CWebrtcFilter::Instance(), pEvent);
}

CWebrtcQtSocketServer* CWebrtcQtSocketServer::Instance(){
    static CWebrtcQtSocketServer* p = NULL;
    if(!p)
        p = new CWebrtcQtSocketServer();
    return p;
}
