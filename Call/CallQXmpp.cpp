#include "CallQXmpp.h"
#include "qxmpp/QXmppUtils.h"
#include "Global/Global.h"
#include "qxmpp/QXmppRtpChannel.h"
#include <QAudioDeviceInfo>

CCallQXmpp::CCallQXmpp(QXmppCall* pCall, bool bVideo, QObject *parent) : CCallObject(bVideo, parent)
{
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
    m_pFrmVideo = NULL;
    m_pCall = pCall;
    if(pCall)
    {
        SetId(QXmppUtils::jidToBareJid(pCall->jid()));
        SetDirection((Direction) pCall->direction());
        ConnectionCallSlot(pCall);
        PlayCallSound();
    }
    else
        Q_ASSERT(false);

    m_VideoThread.setObjectName("VideoProcess");
    m_Camera.moveToThread(&m_VideoThread);
    m_CaptureFrameProcess.moveToThread(&m_VideoThread);
    m_CaptureToRemoteFrameProcess.moveToThread(&m_VideoThread);
    m_ReciveFrameProcess.moveToThread(&m_VideoThread);
}

CCallQXmpp::~CCallQXmpp()
{
    if(m_pAudioInput)
        delete m_pAudioInput;
    if(m_pAudioOutput)
        delete m_pAudioOutput;
    if(m_pFrmVideo)
        m_pFrmVideo->close();
}

int CCallQXmpp::ConnectionCallSlot(QXmppCall *pCall)
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

int CCallQXmpp::Accept()
{
    int nRet = 0;
    StopCallSound();
    if(m_pCall)
        m_pCall->accept();
    else
        return -1;
    return nRet;
}

int CCallQXmpp::Cancel()
{
    int nRet = 0;
    StopCallSound();
    if(m_pCall)
        m_pCall->hangup();
    else
        return -1;
    return nRet;
}

CCallObject::State CCallQXmpp::GetState()
{
    if(m_pCall)
        return (State)m_pCall->state();
    else
        return ConnectingState;
}

void CCallQXmpp::slotConnection()
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallQXmpp::slotConnection");
    StopCallSound();

    //初始始化音频设备  
    StartAudioDevice();

    if(m_bVideo)
        StartVideo();
}

void CCallQXmpp::slotStateChanged(QXmppCall::State state)
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "State:%d", state);
    emit sigUpdate();
}

void CCallQXmpp::slotFinished()
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallVideoQXmpp::slotFinished");
    StopAudioDevice();
    StopCallSound();
    if(m_bVideo)
    {
        StopVideo();
    }
    emit sigFinished(QSharedPointer<CCallObject>(this));
}

//音频模式改变  
void CCallQXmpp::slotAudioModeChanged(QIODevice::OpenMode mode)
{
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CFrmVideo::audioModeChanged:%x", mode);

    if(!(m_pCall && m_pAudioInput && m_pAudioOutput))
        return;

    if(QIODevice::WriteOnly & mode)
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioInput CFrmVideo::audioModeChanged:%xm_pAudioInput->start()", mode);
        m_pAudioInput->start(m_pCall->audioChannel());
    }
    else 
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioInput CFrmVideo::audioModeChanged:%x---m_pAudioInput->stop()", mode);
        m_pAudioInput->stop();
    }

    if(QIODevice::ReadOnly & mode)
    {
        LOG_MODEL_DEBUG("CCallVideoQXmpp", "OpenAudioOutput CFrmVideo::audioModeChanged:%x-----------m_pAudioOutput->start()", mode);
        m_pAudioOutput->start(m_pCall->audioChannel());
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

int CCallQXmpp::StartAudioDevice()
{
    int nRet = 0;
    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(!pAudioChannel)
    {
        LOG_MODEL_WARNING("CCallVideoQXmpp", "CCallVideoQXmpp::StartAudioDevice:don't get audio channel");
        return -1;
    }

    QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
#ifdef DEBUG
    ShowAudioDevices();
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallVideoQXmpp::connected:audio name:%s;id:%d;channels:%d, clockrate:%d, packet time:%d",
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
    if(!lstInputs.isEmpty() && (lstInputs.size() > CGlobal::Instance()->GetAudioInputDevice()))
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
        else if(pAudioChannel->openMode() & QIODevice::WriteOnly ) 
            m_pAudioInput->start(pAudioChannel);
    }

    QList<QAudioDeviceInfo> lstOutputs = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    if(!lstOutputs.isEmpty() && (lstOutputs.size() > CGlobal::Instance()->GetAudioOutputDevice()))
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
        else if(pAudioChannel->openMode() & QIODevice::ReadOnly)
            m_pAudioOutput->start(pAudioChannel);
    }
    return nRet;
}

//停止设备，并删除对象  
int CCallQXmpp::StopAudioDevice()
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
void CCallQXmpp::slotVideoModeChanged(QIODevice::OpenMode mode)
{
    if(!m_pCall)
        return;
    LOG_MODEL_DEBUG("CCallQXmpp", "CCallQXmpp::slotVideoModeChanged:mode:%d", mode);
    if(!m_bVideo)
    {
        m_bVideo = true;
        StartVideo();
    }

    if(QIODevice::WriteOnly == mode)
    {
        //SetVideoFormat();
    }
    if(QIODevice::ReadOnly == mode)
    {
        int t = 1000 / m_pCall->videoChannel()->encoderFormat().frameRate();
        m_tmRecive.start(t);
    }
}

void CCallQXmpp::slotCaptureFrame(const QXmppVideoFrame &frame)
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

void CCallQXmpp::slotReciveFrame()
{
    if(!m_pCall->videoChannel())
        return;
    QList<QXmppVideoFrame> f = m_pCall->videoChannel()->readFrames();
    foreach(QXmppVideoFrame frame, f)
    {
        if(!frame.isValid())
            continue;
        m_ReciveFrameProcess.slotFrameConvertedToRGB32(frame, QRect(0, 0, frame.width(), frame.height()));
    }
}

int CCallQXmpp::SetVideoFormat()
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
    videoFormat.setFrameRate(m_Camera.GetFrameRate());
    LOG_MODEL_DEBUG("CCallQXmpp", "CCallQXmpp::SetVideoFormat:width:%d, height:%d", m_Camera.GetWidth(), m_Camera.GetHeight());
    videoFormat.setFrameSize(QSize(m_Camera.GetWidth(), m_Camera.GetHeight()));
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

int CCallQXmpp::StartVideo()
{
    if(!m_bVideo)
    {
        return -1;
    }
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CCallQXmpp", "CCallQXmpp::StartVideo threadid:0x%X",
           QThread::currentThreadId());
#endif

    bool check = false;
    m_VideoThread.start();//开始视频处理线程  
    //打开显示对话框  
    if(m_pFrmVideo)
    {
        m_pFrmVideo->close();
    }
    m_pFrmVideo = new CFrmVideo();
    if(m_pFrmVideo)
    {
        //窗口关闭时会自己释放内存  
        m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);
        check = connect(m_pFrmVideo, SIGNAL(destroyed()),
                        SLOT(slotFrmVideoClose()));
        Q_ASSERT(check);
        m_pFrmVideo->show();
        m_pFrmVideo->activateWindow();
    }

    //显示本地视频  
    check = connect(&m_Camera, SIGNAL(sigCaptureFrame(QVideoFrame)),
                    &m_CaptureFrameProcess, SLOT(slotFrameConvertedToRGB32(QVideoFrame)));
    Q_ASSERT(check);
    check = connect(&m_CaptureFrameProcess, SIGNAL(sigFrameConvertedToRGB32Frame(QVideoFrame)),
                    m_pFrmVideo, SLOT(slotDisplayLoacleVideo(QVideoFrame)));
    Q_ASSERT(check);

    //从本地到网络  
    check = connect(&m_Camera, SIGNAL(sigCaptureFrame(QVideoFrame)),
                    &m_CaptureToRemoteFrameProcess, SLOT(slotFrameConvertedToYUYV(QVideoFrame)));
    Q_ASSERT(check);
    check = connect(&m_CaptureToRemoteFrameProcess, SIGNAL(sigFrameConvertedToYUYVFrame(QXmppVideoFrame)),
                    SLOT(slotCaptureFrame(QXmppVideoFrame)));
    Q_ASSERT(check);

    //从网络到本地  
    //接收定时器  
    check = connect(&m_tmRecive, SIGNAL(timeout()),
                    SLOT(slotReciveFrame()));
    Q_ASSERT(check);
    //接收后会发送信号进行转换,显示网络视频    
    check = connect(&m_ReciveFrameProcess, SIGNAL(sigFrameConvertedToRGB32Frame(QVideoFrame)),
                    m_pFrmVideo, SLOT(slotDisplayRemoteVideo(QVideoFrame)));
    Q_ASSERT(check);

    //初始化视频设备，并开始视频  
    m_Camera.SetDeviceIndex(CGlobal::Instance()->GetVideoCaptureDevice());
    m_Camera.Start();
    if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->startVideo();

}

int CCallQXmpp::StopVideo()
{
    if(!m_bVideo)
        return -1;
    if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->stopVideo();
    m_Camera.Stop();
    m_tmRecive.stop();
    m_VideoThread.quit();
    if(m_pFrmVideo)
    {
        m_pFrmVideo->close();
        m_pFrmVideo = NULL;
    }
    m_Camera.disconnect();
    m_CaptureFrameProcess.disconnect();
    m_CaptureToRemoteFrameProcess.disconnect();
    m_ReciveFrameProcess.disconnect();
    return 0;
}

void CCallQXmpp::slotFrmVideoClose()
{
    if(m_pFrmVideo)
    {
        CFrmVideo* p = m_pFrmVideo;
        m_pFrmVideo = NULL;
        p->close();
    }
    this->Cancel();
}
