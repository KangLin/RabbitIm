#ifndef TALK_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#define TALK_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "VideoRenderer.h"

namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket

class CCallObjectQXmppWebrtc;
class CWebrtcFilter;
class CWebrtcConductor
      : public webrtc::PeerConnectionObserver,
        public webrtc::CreateSessionDescriptionObserver
{
public:
    CWebrtcConductor(CCallObjectQXmppWebrtc* pCall);
    ~CWebrtcConductor();

    //仅在主线程调用一次  
    static void InitWebrtcGlobal();
    static void CleanWebrtcGlobal();
    int PeerConnect();
    int AcceptConnect(std::string szSdp, std::string szType);
    int ReciveIceCandidate(std::string szMid, int nIndex, std::string szSdp);
    int PeerStop();

    //
    // PeerConnectionObserver implementation.
    //
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnDataChannel(webrtc::DataChannelInterface* channel);
    virtual void OnRenegotiationNeeded();
    virtual void OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state);
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
    virtual void OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state);

    // CreateSessionDescriptionObserver implementation.
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);
    
    int CloseLocaleRander(bool bClose = true);

private:
    bool InitializePeerConnection();
    void DeletePeerConnection();
    bool CreatePeerConnection();
    void AddStreams();
    cricket::VideoCapturer* OpenVideoCaptureDevice();
    
private:
    std::string m_szStreamLable;
    std::string m_szVideoTrackLable;
    std::string m_szAudioTrack;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
        peer_connection_factory_;
    bool m_dtls;
    CCallObjectQXmppWebrtc* m_pCall;
    rtc::scoped_ptr<CVideoRenderer> m_LocaleVideoRender, m_RemoteVideoRender;
    
    static rtc::Thread* m_pSignalThread;
    friend class CWebrtcFilter;
};

#endif  // TALK_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
