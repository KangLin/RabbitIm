#ifndef CWEBRTCQTSOCKETSERVER_H
#define CWEBRTCQTSOCKETSERVER_H

#include "webrtc/base/physicalsocketserver.h"

class CWebrtcConductor;
class CWebrtcQtSocketServer : public rtc::SocketServer
{
public:
    CWebrtcQtSocketServer(CWebrtcConductor* pWebrtcConductor);
    
    virtual bool Wait(int cms, bool process_io);
  
    virtual void WakeUp();
    virtual rtc::Socket* CreateSocket(int type) {
       return NULL;
     }
   
     virtual rtc::Socket* CreateSocket(int family, int type) {
       return NULL;
     }
   
     virtual rtc::AsyncSocket* CreateAsyncSocket(int type) {
       return NULL;
     }
   
     virtual rtc::AsyncSocket* CreateAsyncSocket(int family, int type) {
       return NULL;
     }
    
private:
    CWebrtcConductor* m_pWebrtcConductor;
    
};

#endif // CWEBRTCQTSOCKETSERVER_H
