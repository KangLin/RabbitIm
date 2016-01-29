
#include "WebrtcQtSocketServer.h"
#include "WebrtcConductor.h"
#include <QApplication>
#include "WebrtcFilter.h"

CWebrtcQtSocketServer::CWebrtcQtSocketServer(CWebrtcConductor *pWebrtcConductor)
{
    m_pWebrtcConductor = pWebrtcConductor;
}

bool CWebrtcQtSocketServer::Wait(int cms, bool process_io)
{
    return true;
}

void CWebrtcQtSocketServer::WakeUp()
{
    QEvent *pEvent = new QEvent(QEvent::User);
    qApp->postEvent(m_pWebrtcConductor->m_pWebrtcFilter, pEvent);
}
