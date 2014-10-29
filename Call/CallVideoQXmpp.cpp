#include "CallVideoQXmpp.h"

CCallVideoQXmpp::CCallVideoQXmpp(QXmppCall *pCall, QObject *parent) :
    CCallQXmpp(pCall, parent)
{
}

int CCallVideoQXmpp::ConnectionCallSlot(QXmppCall *pCall)
{
    bool check = false;

    CCallQXmpp::ConnectionCallSlot(pCall);

    check = connect(pCall, SIGNAL(videoModeChanged(QIODevice::OpenMode)),
                    SLOT(slotVideoModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);
    if(check)
        return 0;

    return -1;
}

//视频模式改变  
void CCallVideoQXmpp::slotVideoModeChanged(QIODevice::OpenMode mode)
{
    if(!m_pCall)
        return;
/*
    LOG_MODEL_DEBUG("Video", "CFrmVideo::videoModeChanged:%x", mode);
    if(QIODevice::ReadOnly & mode)
    {
        //int nInterval = (double)1000 / m_pCall->videoChannel()->decoderFormat().frameRate();
        m_VideoPlayTimer.start(10); //TODO:按 10ms 取视频帧，目前没有做缓存抖动处理  
    }
    else
        m_VideoPlayTimer.stop();*/
}
