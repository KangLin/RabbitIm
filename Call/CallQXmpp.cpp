#include "CallQXmpp.h"
#include "qxmpp/QXmppUtils.h"
#include "Global/Global.h"
#include "qxmpp/QXmppRtpChannel.h"
#include <QAudioDeviceInfo>

CCallQXmpp::CCallQXmpp(QXmppCall* pCall, QObject *parent) : CCallObject(parent)
{
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
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
}

CCallQXmpp::~CCallQXmpp()
{
    if(m_pAudioInput)
        delete m_pAudioInput;
    if(m_pAudioOutput)
        delete m_pAudioOutput;
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
    LOG_MODEL_DEBUG("CCallVideoQXmpp", "CCallVideoQXmpp::slotConnection");
    StopCallSound();

    //初始始化音频设备  
    StartAudioDevice();

    //初始化视频设备，并开始视频  
    //StartVideo();
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

//视频模式改变  
void CCallQXmpp::slotVideoModeChanged(QIODevice::OpenMode mode)
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
