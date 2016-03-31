#include "CallObjectQXmpp.h"
#include "qxmpp/QXmppUtils.h"
#include "Global/Global.h"
#include "qxmpp/QXmppRtpChannel.h"
#include <QAudioDeviceInfo>
#include "MainWindow.h"

CCallObjectQXmpp::CCallObjectQXmpp(QXmppCall* pCall,
                                   bool bVideo,
                                   QObject *parent)
    : CCallObject(QXmppUtils::jidToBareJid(pCall->jid()), bVideo, parent)
{
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
    m_pCamera = NULL;
    m_pCall = pCall;
    if(m_pCall)
    {
        SetDirection((Direction) m_pCall->direction());
        ConnectionCallSlot(m_pCall);
    }
    else
        Q_ASSERT(false);

    /*m_VideoThread.setObjectName("VideoProcess");
    m_Camera.moveToThread(&m_VideoThread);
    m_CaptureFrameProcess.moveToThread(&m_VideoThread);
    m_CaptureToRemoteFrameProcess.moveToThread(&m_VideoThread);
    m_ReciveFrameProcess.moveToThread(&m_VideoThread);
    */
    bool check = connect(GET_MAINWINDOW, SIGNAL(sigRefresh()),
                         SLOT(slotUpdateOption()));
    Q_ASSERT(check);
}

CCallObjectQXmpp::~CCallObjectQXmpp()
{
    GET_MAINWINDOW->disconnect(this);
    //TODO:多线程在运行时直接关闭主窗口会core，原因是主窗口关闭后QXMPP库已析构？  
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp status:%d",
                    GetState());

    StopAudioDevice();

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

int CCallObjectQXmpp::Stop()
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
    
    //初始始化音频设备  
    StartAudioDevice();

    if(m_bVideo)
        StartVideo();
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
    StopAudioDevice();
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
#endif
    QAudioFormat inFormat, outFormat;
    inFormat.setSampleRate(AudioPlayLoadType.clockrate());
    inFormat.setChannelCount(AudioPlayLoadType.channels());
    inFormat.setSampleSize(16);
    inFormat.setSampleType(QAudioFormat::SignedInt);
    inFormat.setByteOrder(QAudioFormat::LittleEndian);
    inFormat.setCodec("audio/pcm");

    outFormat = inFormat;

    QList<QAudioDeviceInfo> lstInputs = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!lstInputs.isEmpty() 
            && (lstInputs.size() > CGlobal::Instance()->GetAudioInputDevice())
            && CGlobal::Instance()->GetAudioInputDevice() > -1)
    {
        QAudioDeviceInfo infoAudioInput(lstInputs.at(CGlobal::Instance()->GetAudioInputDevice()));
        if (!infoAudioInput.isFormatSupported(inFormat)) {
            LOG_MODEL_WARNING("CCallVideoQXmpp", "Default audio input format not supported - trying to use nearest");
            //TODO:增加格式转换  
            inFormat = infoAudioInput.nearestFormat(inFormat);
        }
        m_pAudioInput = new QAudioInput(infoAudioInput, inFormat, this);
        if(!m_pAudioInput)
            LOG_MODEL_ERROR("CCallVideoQXmpp", "Create QAudioInput device instance fail.");
        else if((pAudioChannel->openMode() & QIODevice::WriteOnly)  && (m_pAudioInput->state() != QAudio::ActiveState) )
            m_pAudioInput->start(pAudioChannel);
    }

    QList<QAudioDeviceInfo> lstOutputs = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    if(!lstOutputs.isEmpty()
            && (lstOutputs.size() > CGlobal::Instance()->GetAudioOutputDevice())
            && CGlobal::Instance()->GetAudioOutputDevice() > -1)
    {
        QAudioDeviceInfo infoAudioOutput(lstOutputs.at(CGlobal::Instance()->GetAudioOutputDevice()));
        if (!infoAudioOutput.isFormatSupported(outFormat)) {
            LOG_MODEL_WARNING("CCallVideoQXmpp", "Default audio output format not supported - trying to use nearest");
            //TODO:增加格式转换  
            outFormat = infoAudioOutput.nearestFormat(outFormat);
        }
        m_pAudioOutput = new QAudioOutput(infoAudioOutput, outFormat, this);
        if(!m_pAudioOutput)
            LOG_MODEL_ERROR("CCallVideoQXmpp", "Create QAudioOutput device instance fail.");
        else if((pAudioChannel->openMode() & QIODevice::ReadOnly) && (m_pAudioOutput->state() != QAudio::ActiveState) )
            m_pAudioOutput->start(pAudioChannel);
    }
    return nRet;
}

//停止设备，并删除对象  
int CCallObjectQXmpp::StopAudioDevice()
{
    if(m_pAudioInput)
    {
        m_pAudioInput->stop();
        delete m_pAudioInput;
        m_pAudioInput = NULL;
    }

    if(m_pAudioOutput)
    {
        m_pAudioOutput->stop();
        delete m_pAudioOutput;
        m_pAudioOutput = NULL;
    }

    return 0;
}

//视频模式改变  
void CCallObjectQXmpp::slotVideoModeChanged(QIODevice::OpenMode mode)
{
    if(!m_pCall)
        return;
    LOG_MODEL_DEBUG("CCallObjectQXmpp", "CCallObjectQXmpp::slotVideoModeChanged:mode:%d", mode);
    if(!m_bVideo && GetDirection() == IncomingDirection)
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

//摄像头捕获帧  
int CCallObjectQXmpp::OnFrame(const std::shared_ptr<CVideoFrame> frame)
{
    //处理捕获到的帧  
    m_CaptureFrameProcess.slotCaptureFrame(frame);
    return 0;
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
        m_ReciveFrameProcess.slotFrameConvertedToRGB32(
                    frame, QRect(0, 0, frame.width(), frame.height()));
    }
}

//向网络发送视频帧  
void CCallObjectQXmpp::slotCaptureFrame(const QXmppVideoFrame &frame)
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

//    static int nWidth = 0, nHeight = 0;
//    if(frame.width() != nWidth || frame.height() != frame.height())
//    {
//        nWidth = frame.width();
//        nHeight = frame.height();
//        QXmppVideoFormat format = pChannel->decoderFormat();
//        format.setFrameSize(QSize(nWidth, nHeight));
//        pChannel->setEncoderFormat(format);
//    }

    pChannel->writeFrame(frame);
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

    bool check = false;
    //m_VideoThread.start();//开始视频处理线程  

    //从摄像头到网络  
    check = connect(&m_CaptureFrameProcess, SIGNAL(sigCaptureFrame(QVideoFrame)),
                    &m_CaptureFrameProcess,
                    SLOT(slotFrameConvertedToYUYV(QVideoFrame)));
    Q_ASSERT(check);
    check = connect(&m_CaptureFrameProcess,
                    SIGNAL(sigFrameConvertedToYUYVFrame(QXmppVideoFrame)),
                    SLOT(slotCaptureFrame(QXmppVideoFrame)));

    //初始化视频设备，并开始视频  
    m_pCamera = CCameraFactory::Instance()->GetCamera(
                CGlobal::Instance()->GetVideoCaptureDevice());
    try{
        if(m_pCamera)
        {
            if(m_pCamera->Open(this))
                m_pCamera->Start();
        }
    }catch(...){
        LOG_MODEL_ERROR("CCallObjectQXmpp", "Start camera error");
    }

    if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->startVideo();

    //从网络到本地  
    //接收定时器  
    check = connect(&m_tmRecive, SIGNAL(timeout()),
                    SLOT(slotReciveFrame()));
    Q_ASSERT(check);
    //启动接收定时器  
    int t = 1000 / m_pCall->videoChannel()->encoderFormat().frameRate();
    m_tmRecive.start(t);

    return 0;
}

int CCallObjectQXmpp::StopVideo()
{
    if(!m_bVideo)
        return -1;

    m_CaptureFrameProcess.disconnect();
    m_ReciveFrameProcess.disconnect();

    if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->stopVideo();

    if(m_pCamera)
    {
        m_pCamera->Stop();
        m_pCamera->Close();
        m_pCamera = NULL;
    }
    m_tmRecive.stop();
    //m_VideoThread.quit();
    return 0;
}

int CCallObjectQXmpp::ConnectLocaleVideo()
{
    if(!m_bVideo || !m_pFrmVideo)
        return -1;
    //显示本地视频  
    if(CGlobal::Instance()->GetIsShowLocaleVideo())
    {
        bool check = connect(&m_CaptureFrameProcess,
                        SIGNAL(sigCaptureFrame(QVideoFrame)),
                        &m_CaptureFrameProcess,
               SLOT(slotFrameConvertedToRGB32(QVideoFrame)));
        Q_ASSERT(check);

        check = connect(&m_CaptureFrameProcess,
                        SIGNAL(sigFrameConvertedToRGB32Frame(
                                   const QVideoFrame&)),
                        m_pFrmVideo,
                        SLOT(slotDisplayLocaleVideo(
                                 const QVideoFrame&)));
        Q_ASSERT(check);
    }

    return 0;
}

int CCallObjectQXmpp::DisconnectLocaleVideo()
{
    if(!m_bVideo)
        return -1;
    if(m_pFrmVideo)
        m_CaptureFrameProcess.disconnect(m_pFrmVideo);
    return 0;
}

void CCallObjectQXmpp::slotUpdateOption()
{
    DisconnectLocaleVideo();
    if(CGlobal::Instance()->GetIsShowLocaleVideo())
    {
        ConnectLocaleVideo();
    }
}

int CCallObjectQXmpp::OpenVideoWindow()
{
    int nRet = CCallObject::OpenVideoWindow();
    if(nRet)
        return nRet;
    
    if(!m_pFrmVideo)
        return 0;

    //接收后会发送信号进行转换,显示网络视频    
    bool check = connect(&m_ReciveFrameProcess, 
                    SIGNAL(sigFrameConvertedToRGB32Frame(QVideoFrame)),
                    m_pFrmVideo,
                    SLOT(slotDisplayRemoteVideo(QVideoFrame)));
    Q_ASSERT(check);

    //显示本地视频  
    if(CGlobal::Instance()->GetIsShowLocaleVideo())
    {
        ConnectLocaleVideo();
    }
    return 0;
}
