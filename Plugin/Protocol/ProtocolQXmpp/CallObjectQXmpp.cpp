#include "CallObjectQXmpp.h"
#include "QXmppUtils.h"
#include "Global/Global.h"
#include "QXmppRtpChannel.h"
#include <QAudioDeviceInfo>
#include "MainWindow.h"

CCallObjectQXmpp::CCallObjectQXmpp(QXmppCall* pCall,
                                   bool bVideo,
                                   QObject *parent)
    : CCallObject(QXmppUtils::jidToBareJid(pCall->jid()), bVideo, parent)
{
    m_pCall = pCall;
    if(m_pCall)
    {
        SetDirection((Direction) m_pCall->direction());
        ConnectionCallSlot(m_pCall);
    }
    else
        Q_ASSERT(false);
}

CCallObjectQXmpp::~CCallObjectQXmpp()
{
    GET_MAINWINDOW->disconnect(this);
    //TODO:多线程在运行时直接关闭主窗口会core，原因是主窗口关闭后QXMPP库已析构？   
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp status:%d",
                    GetState());

    slotFinished();

    LOG_MODEL_DEBUG("CCallObjectQXmpp",
                    "CCallObjectQXmpp::~CCallObjectQXmpp.id:%d",
                    qPrintable(GetId()));
}

int CCallObjectQXmpp::ConnectionCallSlot(QXmppCall *pCall)
{
    bool check = false;
    //只有主叫方才有的事件  
    /*check = connect(pCall, SIGNAL(ringing()),
                    SLOT(ringing()));
    Q_ASSERT(check);*/
    
    //以下是双方都有的事件  
    check = connect(pCall, SIGNAL(connected()),
                         SLOT(slotConnection()));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(stateChanged(QXmppCall::State)),
                    SLOT(slotStateChanged(QXmppCall::State)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(audioModeChanged(QIODevice::OpenMode)),
                    SLOT(slotAudioModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(videoModeChanged(QIODevice::OpenMode)),
                    SLOT(slotVideoModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(finished()),
                    SLOT(slotFinished()));
    Q_ASSERT(check);

    if(check)
        return 0;

    return -1;
}

int CCallObjectQXmpp::Accept()
{
    int nRet = 0;
    if(!m_pCall)
        return -1;
    
    m_pCall->accept();   
    return nRet;
}

int CCallObjectQXmpp::Stop(StopState state)
{
    int nRet = 0;
    if(!m_pCall)
        return -1;
        
    m_pCall->hangup();
    slotChanageState(FinishedState);
    return nRet;
}

void CCallObjectQXmpp::slotConnection()
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallObjectQXmpp::slotConnection");
    
    if(m_bVideo && this->GetDirection() == OutgoingDirection)
        StartVideo();
    
    //初始始化音频设备  
    StartAudioDevice();
}

void CCallObjectQXmpp::slotStateChanged(QXmppCall::State state)
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "State:%d", state);
    m_State = (State) state;
    slotChanageState(m_State);
}

void CCallObjectQXmpp::slotFinished()
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallVideoQXmpp::slotFinished");
    if(m_bVideo)
    {
        StopVideo();
    }
}

//音频模式改变  
void CCallObjectQXmpp::slotAudioModeChanged(QIODevice::OpenMode mode)
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CFrmVideo::audioModeChanged:%x", mode);

    if(!(m_pCall && m_pAudioInput && m_pAudioOutput))
        return;

    if(QIODevice::WriteOnly & mode) 
    {
        if (m_pAudioInput->state() != QAudio::ActiveState)
        {
            LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioInput CFrmVideo::audioModeChanged:%xm_pAudioInput->start()", mode);
            m_pAudioInput->start(m_pCall->audioChannel());
        }
    }
    else 
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioInput CFrmVideo::audioModeChanged:%x---m_pAudioInput->stop()", mode);
        m_pAudioInput->stop();
    }

    if(QIODevice::ReadOnly & mode)
    {
        if (m_pAudioOutput->state() != QAudio::ActiveState)
        {
            LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioOutput CFrmVideo::audioModeChanged:%x-----------m_pAudioOutput->start()", mode);
            m_pAudioOutput->start(m_pCall->audioChannel());
        }
    }
    else
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioOutput CFrmVideo::audioModeChanged:%x-----------m_pAudioOutput->stop()", mode);
        m_pAudioOutput->stop();
    }
}

void ShowAudioDeviceSupportCodec(QAudioDeviceInfo &info, QString szPropmt)
{
    qDebug("==================== %s start ========================", qPrintable(szPropmt));
    qDebug("device name:%s", qPrintable(info.deviceName()));
    QString szTemp;
    foreach(QString codec, info.supportedCodecs())
    {        
        szTemp += codec;
    }
    qDebug("audio device support codec:%s, ", qPrintable(szTemp));
    szTemp.clear();
    foreach (int sampleRate, info.supportedSampleRates()) {
        szTemp = szTemp + QString::number(sampleRate) + ", ";
    }
    qDebug("audio device support sample rate:%s", qPrintable(szTemp));
    szTemp.clear();
    foreach (int channel, info.supportedChannelCounts()) {
        szTemp = szTemp + QString::number(channel) + ", ";
    }
    qDebug("audio device support channel:%s", qPrintable(szTemp));
    szTemp.clear();
    foreach(int size, info.supportedSampleSizes())
    {
        szTemp = szTemp + QString::number(size) + ", ";
    }
    qDebug("audio device support sample size:%s", qPrintable(szTemp));
    szTemp.clear();
    foreach(QAudioFormat::SampleType type, info.supportedSampleTypes())
    {
        szTemp = szTemp + QString::number(type) + ", ";
    }
    qDebug("audio device support sample type:%s", qPrintable(szTemp));
    szTemp.clear();
    foreach(int order, info.supportedByteOrders())
    {
        szTemp = szTemp + QString::number(order) + ", ";
    }
    qDebug("audio device support byte order:%s", qPrintable(szTemp));

    qDebug("==================== %s end========================", qPrintable(szPropmt));
}

void ShowAudioDevices()
{
    int n = 0;
    QList<QAudioDeviceInfo> infos = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo info, infos) {
        ShowAudioDeviceSupportCodec(info, "input" + QString::number(++n));
    }
    QAudioDeviceInfo in = QAudioDeviceInfo::defaultInputDevice();
    ShowAudioDeviceSupportCodec(in, "default input");
    n = 0;
    infos = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo info, infos) {
        ShowAudioDeviceSupportCodec(info, "output" + QString::number(++n));
    }
    QAudioDeviceInfo out = QAudioDeviceInfo::defaultOutputDevice();
    ShowAudioDeviceSupportCodec(out, "default output");
}

int CCallObjectQXmpp::StartAudioDevice()
{
    int nRet = 0;
    if(!m_pCall)
    {
        LOG_MODEL_ERROR("CCallObjectQXmpp", "CCallObjectQXmpp::StartAudioDevice is null");
        return -1;
    }

    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(!pAudioChannel)
    {
        LOG_MODEL_WARNING("CCallVideoQXmpp", "CCallVideoQXmpp::StartAudioDevice:don't get audio channel");
        return -2;
    }

    QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
    /*
#ifdef DEBUG
    ShowAudioDevices();
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallVideoQXmpp::connected:audio payload name:%s;id:%d;channels:%d, clockrate:%d, packet time:%d",
           qPrintable(AudioPlayLoadType.name()),
           AudioPlayLoadType.id(),
           AudioPlayLoadType.channels(),
           AudioPlayLoadType.clockrate(),
           AudioPlayLoadType.ptime());
    QMap<QString, QString>::iterator it;
    for(it = AudioPlayLoadType.parameters().begin(); it != AudioPlayLoadType.parameters().end(); it++)
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "parameter:%s;value:%s" , qPrintable(it.key()),qPrintable(it.value()));
    }
#endif //*/
    QAudioFormat inFormat, outFormat;
    inFormat.setSampleRate(AudioPlayLoadType.clockrate());
    inFormat.setChannelCount(AudioPlayLoadType.channels());
    inFormat.setSampleSize(16);
    inFormat.setSampleType(QAudioFormat::SignedInt);
    inFormat.setByteOrder(QAudioFormat::LittleEndian);
    inFormat.setCodec("audio/pcm");

    outFormat = inFormat;

    nRet = OpenAudioDevice(inFormat, outFormat, pAudioChannel);
    return nRet;
}

//视频模式改变  
void CCallObjectQXmpp::slotVideoModeChanged(QIODevice::OpenMode mode)
{
    if(!m_pCall)
        return;
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp::slotVideoModeChanged:mode:%d", mode);
    if(!m_bVideo && GetDirection() == IncomingDirection
            && (QIODevice::ReadOnly & mode))
    {
        m_bVideo = true;
        StartVideo();
        slotChanageState(ActiveState);
    }

    if(QIODevice::WriteOnly == mode)
    {
        //SetVideoFormat();
    }
    if(QIODevice::ReadOnly == mode)
    {
    }
}

//从网络上接收视频帧  
void CCallObjectQXmpp::slotReciveFrame()
{
    if(!m_pCall->videoChannel())
        return;
    QList<QXmppVideoFrame> f = m_pCall->videoChannel()->readFrames();
    //监控模式下接收后直接返回  
    if(IsMonitor())
        return;

    foreach(QXmppVideoFrame frame, f)
    {
        if(!frame.isValid())
            continue;
        QImage image;

        emit sigRenderRemote(image);
    }
}

//向网络发送视频帧  
void CCallObjectQXmpp::soltVideoFrameToRemote(const QVideoFrame &frame)
{
    if(!m_pCall)
    {
        LOG_MODEL_DEBUG("Video", "m_pCall is NULL");
        return;
    }

    QXmppRtpVideoChannel *pChannel = m_pCall->videoChannel();
    if(!pChannel || !(pChannel->openMode() & QIODevice::WriteOnly) || !frame.isValid())
    {
        LOG_MODEL_DEBUG("Video", "m_pCall->videoChannel() is null or openMode isn't write mode");
        return;
    }
    
    if(QVideoFrame::Format_YUYV != frame.pixelFormat())
    {
        LOG_MODEL_ERROR("Call", "frame.pixelFormat(%d) isn't QVideoFrame::Format_YUYV", frame.pixelFormat());
        return;
    }

//    static int nWidth = 0, nHeight = 0;
//    if(frame.width() != nWidth || frame.height() != frame.height())
//    {
//        nWidth = frame.width();
//        nHeight = frame.height();
//        QXmppVideoFormat format = pChannel->decoderFormat();
//        format.setFrameSize(QSize(nWidth, nHeight));
//        pChannel->setEncoderFormat(format);
//    }

    
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;
#ifdef RABBITIM_USE_FFMPEG
    QXmppVideoFrame outFrame(inFrame.mappedBytes(),
                             inFrame.size(),
                             inFrame.bytesPerLine(),
        CTool::QVideoFrameFormatToQXmppVideoFrameFormat(inFrame.pixelFormat()));

    //TODO:这里多了一次内存复制  
    memcpy(outFrame.bits(), inFrame.bits(), inFrame.mappedBytes());
    pChannel->writeFrame(outFrame);
#endif
    inFrame.unmap();
}

int CCallObjectQXmpp::SetVideoFormat()
{
    QXmppVideoFormat videoFormat;
    // QXmpp uses this defaults formats for Encoder/Decoder:
    //
    // Default Decoder Format
    // {
    //     frameRate =  15
    //     frameSize =  QSize(320, 240)
    //     pixelFormat =  18
    // }
    //
    // Default Encoder Format
    // {
    //     frameRate =  15
    //     frameSize =  QSize(320, 240)
    //     pixelFormat =  21
    // }
    /*videoFormat.setFrameRate(m_Camera.GetFrameRate());
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp::SetVideoFormat:width:%d, height:%d", m_Camera.GetWidth(), m_Camera.GetHeight());
    videoFormat.setFrameSize(QSize(m_Camera.GetWidth(), m_Camera.GetHeight()));*/
    // QXmpp allow the following pixel formats for video encoding:
    //
    // PixelFormat
    // {
    //     Format_Invalid = 0,
    //     Format_RGB32 = 3,
    //     Format_RGB24 = 4,
    //     Format_YUV420P = 18,
    //     Format_UYVY = 20,
    //     Format_YUYV = 21
    // }
    //
    // QXmpp can be compiled with Vp8 and Theora support.
    // The encoding formats supported by this codecs are:
    //
    // Vpx    -> QXmppVideoFrame::Format_YUYV
    //
    // Theora -> QXmppVideoFrame::Format_YUV420P
    //           QXmppVideoFrame::Format_YUYV

    videoFormat.setPixelFormat(QXmppVideoFrame::Format_YUYV);

    // Change default Encoder Format.
    m_pCall->videoChannel()->setEncoderFormat(videoFormat);
    return 0;
}

int CCallObjectQXmpp::StartVideo()
{
    if(!m_bVideo || !m_pCall)
    {
        return -1;
    }
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp::StartVideo threadid:0x%X",
           QThread::currentThreadId());
#endif

    if(m_pCamera)
    {
        LOG_MODEL_WARNING("Call", "Camera is started");
        return -2;
    }

    bool check = false;

    m_pCall->startVideo();

    //从网络到本地  
    //接收定时器  
    check = connect(&m_tmRecive, SIGNAL(timeout()),
                    SLOT(slotReciveFrame()));
    Q_ASSERT(check);
    //TODO: 启动接收定时器  是否单独启用一个线程？
    int t = 1000 / m_pCall->videoChannel()->encoderFormat().frameRate();
    m_tmRecive.start(t);

    return 0;
}

int CCallObjectQXmpp::StopVideo()
{
    if(!m_bVideo)
        return -1;

    //if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->stopVideo();

    m_tmRecive.stop();
    //m_VideoThread.quit();
    return 0;
}
