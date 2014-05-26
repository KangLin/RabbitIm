#include "../../Tool.h"
#include "FrmVideo.h"
#include "ui_FrmVideo.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QPainter>
#include <QAbstractVideoBuffer>
#include <QMetaType>
#include <QTime>
#include <QSound>
#include <QLinearGradient>
#include <QRadialGradient>
#include "../FrmUserList/Roster.h"
#include "../../Global.h"
#include "qxmpp/QXmppRtpChannel.h"

extern CGlobal g_Global;

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    m_RemotePlayer(this),
    m_LocalePlayer(this),
    m_CaptureVideoFrame(this),
    ui(new Ui::CFrmVideo)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::CFrmVideo");
    ui->setupUi(this);

    //设置提示文本颜色
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    ui->lbPrompt->setPalette(pe);

    m_bCall = false;
    m_pCall = NULL;
    m_pClient = NULL;
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
    m_pCallSound = NULL;
    m_pCamera = NULL;

    m_VideoThread.start();
    //m_AudioRecordInput.moveToThread(&m_AudioThread);
    //m_AudioRecordOutput.moveToThread(&m_AudioThread);
    //m_AudioThread.start();
}

CFrmVideo::~CFrmVideo()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::~CFrmVideo");
    m_VideoThread.quit();
    m_VideoThread.wait();
    m_AudioThread.quit();
    m_AudioThread.wait();

    //其它资源释放在closeEvent中进行

    if(m_pClient)
    {
        //m_pClient->m_CallManager.disconnect(this);
        //m_pClient->disconnect(this);
    }//TODO:释放连接，但会引起core dump*/

    delete ui;

    LOG_MODEL_DEBUG("Video", "CFrmVideo::~CFrmVideo end");
}

QThread* CFrmVideo::GetVideoThread()
{
    return &m_VideoThread;
}

CFrmVideo* CFrmVideo::instance(CXmppClient *pClient)
{
    static CFrmVideo *pVideo = NULL;
    if(NULL == pVideo)
    {
        pVideo = new CFrmVideo();
        pVideo->SetClient(pClient);
    }
    return pVideo;
}

int CFrmVideo::SetClient(CXmppClient *pClient)
{
    m_pClient = pClient;

    bool check = connect(&m_pClient->m_CallManager,
                         SIGNAL(callReceived(QXmppCall*)),
                         SLOT(callReceived(QXmppCall*)));
    Q_ASSERT(check);

    check = connect(&m_pClient->m_CallManager,
                    SIGNAL(callStarted(QXmppCall*)),
                    SLOT(callStarted(QXmppCall*)));
    Q_ASSERT(check);

    check = connect(m_pClient, SIGNAL(iqReceived(QXmppIq)),
                    SLOT(clientIqReceived(QXmppIq)));
    Q_ASSERT(check);

    //连接本地视频头捕获视频帧信号到本地播放视频窗口
    check = connect(&m_CaptureVideoFrame, SIGNAL(sigCaptureFrame(const QVideoFrame&)),
                    &m_LocalePlayer, SLOT(slotPresent(const QVideoFrame&)));
    Q_ASSERT(check);

    //连接到网络发送
    check = connect(&m_CaptureVideoFrame, SIGNAL(sigConvertedToYUYVFrame(const QXmppVideoFrame&)),
                    SLOT(slotCaptureFrame(const QXmppVideoFrame&)));
    Q_ASSERT(check);

    //连接从网络接收视频播放定时器
    check = connect(&m_VideoPlayTimer, SIGNAL(timeout()),
                    SLOT(slotUpdateReciverVideo()));
    Q_ASSERT(check);

    return 0;
}

void CFrmVideo::clientIqReceived(const QXmppIq &iq)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::clientIqReceived:%d", iq.error().condition());
}

void CFrmVideo::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Video", "CFrmVideo::closeEvent");
    if(m_pCall)
    {
        m_pCall->hangup();
    }
    //立即停止播放声音
    StopCallSound();
}

//调整视频播放窗口的位置
void CFrmVideo::AdjustPlayer(const QRect &rect)
{
    m_RemotePlayer.setGeometry(rect);
    QPoint point(rect.topLeft());
    point.setX(point.x() + (rect.width() - (rect.width() >> 2)));
    point.setY(point.y() + (rect.height() - (rect.height() >> 2)));
    QRect localeRect(point, rect.bottomRight());
    m_LocalePlayer.setGeometry(localeRect);
}

void CFrmVideo::resizeEvent(QResizeEvent *)
{
    //得到视频的高宽
    double nWidth = 320;
    double nHeight = 240;
    if(m_pCall)
    {
        QXmppRtpVideoChannel * pChannel = m_pCall->videoChannel();
        if(pChannel)
        {
            nHeight = pChannel->decoderFormat().frameHeight();
            nWidth = pChannel->decoderFormat().frameWidth();
            qDebug("nHeight:%d, nWidth:%d", nHeight, nWidth);
        }
    }

    QRect rect = this->rect();
    QRect rectAdjust = this->rect();

    //计算宽度
    double frameAspectRatio = nWidth / nHeight;
    double frmAspectRatio = (double)rect.width() / (double)rect.height();
    if(frameAspectRatio < frmAspectRatio)
    {
        //以窗体高度为基准计算宽度
        int nW = rect.height() * frameAspectRatio;
        rectAdjust.setLeft(rect.left() + ((rect.width() - nW) >> 1));
        rectAdjust.setRight(rect.right() - ((rect.width() - nW) >> 1));
    }
    else
    {
        //以宽度为基准计算高度
        double ratio = nHeight / nWidth;
        int nH = rect.width() * ratio;
        rectAdjust.setTop(rect.top() + ((rect.height() - nH) >> 1));
        rectAdjust.setBottom(rect.bottom() - ((rect.height() - nH) >> 1));
    }

    AdjustPlayer(rectAdjust);
}

void CFrmVideo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //QLinearGradient gradient(0, rect().height() >> 2, rect().right(), this->rect().height() >> 2);
    /*
    QRadialGradient gradient(rect().width() >> 2, rect().height() >> 2,
                             rect().height() >> 2,
                             rect().width() >> 2, rect().height() >> 2);//*/
    /*
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(0.5, Qt::green);
    gradient.setColorAt(1, Qt::blue);

    QBrush bg(gradient);//*/
    QBrush bg(Qt::darkBlue);
    painter.setBrush(bg);
    painter.drawRect(rect());
}

//主动发起呼叫
int CFrmVideo::Call(QString jid)
{
    //如果已经有调用，则停止并释放
    if(m_pCall)
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Call"),
                        tr("%1 is talking, Do you stop it?").arg(QXmppUtils::jidToUser(m_pCall->jid())),
                        QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == msg.exec())
        {
            m_pCall->hangup();
        }
        else
            return -1;
    }

    QString szText = tr("%1 is ringing").arg(QXmppUtils::jidToUser(jid));
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);

    m_pClient->m_CallManager.setStunServer(
                QHostAddress(g_Global.GetStunServer()),
                g_Global.GetStunServerPort()
                );
    m_pClient->m_CallManager.setTurnServer(
                QHostAddress(g_Global.GetStunServer()),
                g_Global.GetTurnServerPort()
                );
    m_pClient->m_CallManager.setTurnUser(
                g_Global.GetTurnServerUser()
                );
    m_pClient->m_CallManager.setTurnPassword(
                g_Global.GetTurnServerPassword()
                );
    m_pCall = m_pClient->m_CallManager.call(jid);
    if(NULL == m_pCall)
    {
        LOG_MODEL_ERROR("Video",  "call fail");
        return -2;
    }

    m_bCall = true;

    //播放铃音
    PlayCallSound();

    return 0;
}

//接收呼叫
void CFrmVideo::callReceived(QXmppCall *pCall)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::callReceived:%x", pCall);
    if(m_pCall)
        LOG_MODEL_WARNING("Video", qPrintable(tr("Is calling ") + QXmppUtils::jidToBareJid(m_pCall->jid())));

    m_pCall = pCall;//这里可以不需要，因为用户可能拒绝接收呼叫，在callStarted才是真正呼叫开始
    m_pClient->m_CallManager.setStunServer(
                QHostAddress(g_Global.GetStunServer()),
                g_Global.GetStunServerPort()
                );
    m_pClient->m_CallManager.setTurnServer(
                QHostAddress(g_Global.GetStunServer()),
                g_Global.GetTurnServerPort()
                );
    m_pClient->m_CallManager.setTurnUser(
                g_Global.GetTurnServerUser()
                );
    m_pClient->m_CallManager.setTurnPassword(
                g_Global.GetTurnServerPassword()
                );

    //播放铃音
    PlayCallSound();

    //TODO:增加判断自动接收呼叫用户
    QMessageBox msg(QMessageBox::Question,
                    tr("Call"),
                    tr("%1 is calling ").arg(QXmppUtils::jidToUser(pCall->jid())),
                    QMessageBox::Yes | QMessageBox::No);
    if(QMessageBox::Yes == msg.exec())
    {
        QString szText = tr("Be connecting %1").arg(QXmppUtils::jidToUser(m_pCall->jid()));
        this->setWindowTitle(szText);
        ui->lbPrompt->setText(szText);
        pCall->accept();
    }
    else
        pCall->hangup();

    //停止播放铃音
    StopCallSound();
}

//呼叫开始
void CFrmVideo::callStarted(QXmppCall *pCall)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::callStarted:%x", pCall);
    if(m_pCall != pCall)
        LOG_MODEL_WARNING("Video", "CFrmVideo::callStarted m_pCall != pCall");

    m_pCall = pCall;
    bool check = connect(pCall, SIGNAL(connected()),
                         SLOT(connected()));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(finished()),
                    SLOT(finished()));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(ringing()),
                    SLOT(ringing()));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(stateChanged(QXmppCall::State)),
                    SLOT(stateChanged(QXmppCall::State)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(audioModeChanged(QIODevice::OpenMode)),
                    SLOT(audioModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(videoModeChanged(QIODevice::OpenMode)),
                    SLOT(videoModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    //显示本窗口
    show();
}

//被叫正在响铃(只有主叫方才有)
void CFrmVideo::ringing()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::ringing");
    if(m_pCall)
    {
        QString szText = tr("%1 is ringing").arg(QXmppUtils::jidToUser(m_pCall->jid()));
        this->setWindowTitle(szText);
        ui->lbPrompt->setText(szText);
    }
}

//呼叫状态发生改变
void CFrmVideo::stateChanged(QXmppCall::State state)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::stateChanged:%d", state);
}

void ShowAudioDeviceSupportCodec(QAudioDeviceInfo &info, QString szPropmt)
{
    qDebug("==================== %s ========================", qPrintable(szPropmt));
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

    qDebug("==================== %s ========================", qPrintable(szPropmt));
}

//会话建立后触发
void CFrmVideo::connected()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::connected");

    QString szText = tr("Be talking %1").arg(QXmppUtils::jidToUser(m_pCall->jid()));
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);

    //停止铃音
    StopCallSound();

    //StopAudioDevice();
    //StartAudioDevice();

    StartVideo();
}

//音频模式改变
void CFrmVideo::audioModeChanged(QIODevice::OpenMode mode)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::audioModeChanged:%x", mode);

    if(QIODevice::WriteOnly & mode)
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioInput CFrmVideo::audioModeChanged:%x", mode);
        OpenAudioInput();
    }

    if(QIODevice::ReadOnly & mode)
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioOutput CFrmVideo::audioModeChanged:%x", mode);
        OpenAudioOutput();
    }
}

//视频模式改变
void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::videoModeChanged:%x", mode);
    if(QIODevice::ReadOnly & mode && m_pCall)
    {
        int nInterval = (double)1000 / m_pCall->videoChannel()->decoderFormat().frameRate();
        m_VideoPlayTimer.start(nInterval);
#ifdef DEBUG_VIDEO_TIME
        LOG_MODEL_DEBUG("Video", "Interval:%d;frameRate:%d", nInterval,
                  m_pCall->videoChannel()->decoderFormat().frameRate());
#endif
    }
}

//呼叫结束时触发
void CFrmVideo::finished()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::finished");

    StopCallSound();
    m_bCall = false;
    StopAudioDevice();

    if(m_pCall)
    {
        QString szMsg = tr("Close the connection with %1").arg(QXmppUtils::jidToUser(m_pCall->jid()));

        StopVideo();
        m_VideoPlayTimer.stop();

        m_pCall->disconnect(this);//删除所有连接
        m_pCall->deleteLater();//需要应用程序释放内存
        m_pCall = NULL;

        this->setWindowTitle(szMsg);
        ui->lbPrompt->setText(szMsg);
        /*QMessageBox msg(QMessageBox::Question,
                        tr("Call"),
                        szMsg,
                        QMessageBox::Yes);
        msg.exec();*/
    }
}

int CFrmVideo::StartVideo()
{
    //开始视频
    if(m_bCall)
        m_pCall->startVideo();

    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    for(it = device.begin(); it != device.end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "Camera:%s", qPrintable(QCamera::deviceDescription(*it)));
    }
    if(m_pCamera)
    {
        m_pCamera->stop();
        delete m_pCamera;
    }
#ifdef ANDROID
    if(device.size() == 2)
    {
        m_pCamera = new QCamera(QCamera::availableDevices().at(1));
    }
    else
        m_pCamera = new QCamera;
#else
    m_pCamera = new QCamera;
#endif
    if(!m_pCamera)
    {
        QMessageBox msg(QMessageBox::Warning,
                        tr("Call"),
                        tr("Hasn't camera"),
                        QMessageBox::Yes);
        msg.exec();
        return -1;
    }

    m_pCamera->setCaptureMode(QCamera::CaptureVideo);
    m_CaptureVideoFrame.setSource(m_pCamera);
    m_pCamera->start();

    ui->lbPrompt->hide();
    resizeEvent(NULL);
    m_RemotePlayer.show();
    m_RemotePlayer.setWindowTitle(QXmppUtils::jidToUser(m_pCall->jid()));

    m_LocalePlayer.setWindowTitle(g_Global.GetName());
    m_LocalePlayer.raise();//提升到父窗口中栈的顶部
    m_LocalePlayer.show();
    m_LocalePlayer.activateWindow();

    return 0;
}

int CFrmVideo::StopVideo()
{
    if(m_pCamera)
    {
        m_pCamera->stop();
        delete m_pCamera;
        m_pCamera = NULL;
    }

    m_LocalePlayer.close();
    m_RemotePlayer.close();
    m_pCall->stopVideo();
    ui->lbPrompt->show();
    return 0;
}

int CFrmVideo::OpenAudioInput()
{
    if(!m_pCall)
    {
        LOG_MODEL_WARNING("Video", "m_pCall is null");
        return -1;
    }
    
    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(!pAudioChannel)
    {
        LOG_MODEL_WARNING("Video", "CFrmVideo::StartAudioDevice:don't get audio channel");
        return -2;
    }

    QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
    LOG_MODEL_DEBUG("Video", "CFrmVideo::connected:audio name:%s;id:%d;channels:%d, clockrate:%d, packet time:%d",
           qPrintable(AudioPlayLoadType.name()),
           AudioPlayLoadType.id(),
           AudioPlayLoadType.channels(),
           AudioPlayLoadType.clockrate(),
           AudioPlayLoadType.ptime());
    QMap<QString, QString>::iterator it;
    for(it = AudioPlayLoadType.parameters().begin(); it != AudioPlayLoadType.parameters().end(); it++)
    {
         LOG_MODEL_DEBUG("Video", "parameter:%s;value:%s" , qPrintable(it.key()),qPrintable(it.value()));
    }

    QAudioFormat format;
    format.setSampleRate(AudioPlayLoadType.clockrate());
    format.setChannelCount(AudioPlayLoadType.channels());
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultInputDevice());
    if (!deviceInfo.isFormatSupported(format)) {
        LOG_MODEL_WARNING("Video", "Default audio input format not supported - trying to use nearest");
        format = deviceInfo.nearestFormat(format);
    }
    ShowAudioDeviceSupportCodec(deviceInfo, "input device");

    m_pAudioInput = new QAudioInput(deviceInfo, format, this);
    if(!m_pAudioInput)
    {
        LOG_MODEL_ERROR("Video", "new QAudioInput fail");
        return -3;
    }

    //TODO:录音功能 
    QString szRecordFile;
#ifndef ANDROID
    szRecordFile = g_Global.GetDirUserData();
#else
    szRecordFile = "/sdcard";
#endif

    //*
    if(m_pAudioInput
       && (pAudioChannel->openMode() & QIODevice::WriteOnly))
    {
        LOG_MODEL_DEBUG("Video", "m_pAudioInput->start;threadid:0x%x", QThread::currentThreadId());

        //m_pAudioInput->start(pAudioChannel);
        m_AudioRecordInput.open(QIODevice::WriteOnly, pAudioChannel, NULL, szRecordFile + "/savein.raw");
        m_pAudioInput->start(&m_AudioRecordInput);
    }//*/
    
    return 0;
}

int CFrmVideo::OpenAudioOutput()
{
    if(!m_pCall)
    {
        LOG_MODEL_WARNING("Video", "m_pCall is null");
        return -1;
    }
    
    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(!pAudioChannel)
    {
        LOG_MODEL_WARNING("Video", "CFrmVideo::StartAudioDevice:don't get audio channel");
        return -2;
    }

    QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
    LOG_MODEL_DEBUG("Video", "CFrmVideo::connected:audio name:%s;id:%d;channels:%d, clockrate:%d, packet time:%d",
           qPrintable(AudioPlayLoadType.name()),
           AudioPlayLoadType.id(),
           AudioPlayLoadType.channels(),
           AudioPlayLoadType.clockrate(),
           AudioPlayLoadType.ptime());
    QMap<QString, QString>::iterator it;
    for(it = AudioPlayLoadType.parameters().begin(); it != AudioPlayLoadType.parameters().end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "parameter:%s;value:%s" , qPrintable(it.key()),qPrintable(it.value()));
    }

    QAudioFormat format;
    format.setSampleRate(AudioPlayLoadType.clockrate());
    format.setChannelCount(AudioPlayLoadType.channels());
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    if (!deviceInfo.isFormatSupported(format)) {
        LOG_MODEL_WARNING("Video", "Default audio input format not supported - trying to use nearest");
        format = deviceInfo.nearestFormat(format);
    }
    ShowAudioDeviceSupportCodec(deviceInfo, "output device");

    m_pAudioOutput = new QAudioOutput(deviceInfo, format, this);
    if(!m_pAudioOutput)
    {
        LOG_MODEL_ERROR("Video", "new QAudioInput fail");
        return -3;
    }

    //TODO:录音功能 
    QString szRecordFile;
#ifndef ANDROID
    szRecordFile = g_Global.GetDirUserData();
#else
    szRecordFile = "/sdcard";
#endif

    //*
    if(m_pAudioOutput && m_pCall
        && (pAudioChannel->openMode() & QIODevice::ReadOnly))
    {
        LOG_MODEL_DEBUG("Video", "m_pAudioOutput->start");

        //push模式
        //m_pAudioOutput->start(pAudioChannel);
        
        //push模式
        //m_AudioRecordOutput.open(QIODevice::ReadWrite, pAudioChannel, NULL, szRecordFile + "/saveout.raw");
        //m_pAudioOutput->start(&m_AudioRecordOutput);
        
        //pull模式
        QIODevice* pOut = m_pAudioOutput->start();
        m_AudioRecordOutput.open(QIODevice::ReadWrite, pAudioChannel, pOut, szRecordFile + "/saveout.raw");
        
    }//*/
    
    return 0;
}

int CFrmVideo::StartAudioDevice()
{
    int nRet = 0;
    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(!pAudioChannel)
    {
        LOG_MODEL_WARNING("Video", "CFrmVideo::StartAudioDevice:don't get audio channel");
        return -1;
    }

    QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
    LOG_MODEL_DEBUG("Video", "CFrmVideo::connected:audio name:%s;id:%d;channels:%d, clockrate:%d, packet time:%d",
           qPrintable(AudioPlayLoadType.name()),
           AudioPlayLoadType.id(),
           AudioPlayLoadType.channels(),
           AudioPlayLoadType.clockrate(),
           AudioPlayLoadType.ptime());
    QMap<QString, QString>::iterator it;
    for(it = AudioPlayLoadType.parameters().begin(); it != AudioPlayLoadType.parameters().end(); it++)
    {
        LOG_MODEL_DEBUG("Video", "parameter:%s;value:%s" , qPrintable(it.key()),qPrintable(it.value()));
    }

    QAudioFormat inFormat, outFormat;
    inFormat.setSampleRate(AudioPlayLoadType.clockrate());
    inFormat.setChannelCount(AudioPlayLoadType.channels());
    inFormat.setSampleSize(16);
    inFormat.setSampleType(QAudioFormat::SignedInt);
    inFormat.setByteOrder(QAudioFormat::LittleEndian);
    inFormat.setCodec("audio/pcm");

    outFormat = inFormat;

    QAudioDeviceInfo infoAudioInput(QAudioDeviceInfo::defaultInputDevice());
    if (!infoAudioInput.isFormatSupported(inFormat)) {
        LOG_MODEL_WARNING("Video", "Default audio input format not supported - trying to use nearest");
        inFormat = infoAudioInput.nearestFormat(inFormat);
    }
    ShowAudioDeviceSupportCodec(infoAudioInput, "input device");

    QAudioDeviceInfo infoAudioOutput(QAudioDeviceInfo::defaultOutputDevice());
    if (!infoAudioOutput.isFormatSupported(outFormat)) {
        LOG_MODEL_WARNING("Video", "Default audio output format not supported - trying to use nearest");
        outFormat = infoAudioOutput.nearestFormat(outFormat);
    }
    ShowAudioDeviceSupportCodec(infoAudioOutput, "output device");

    m_pAudioInput = new QAudioInput(infoAudioInput, inFormat, this);
    m_pAudioOutput = new QAudioOutput(infoAudioOutput, outFormat, this);

    //TODO:录音功能 
    QString szRecordFile;
#ifndef ANDROID
    szRecordFile = g_Global.GetDirUserData();
#else
    szRecordFile = "/sdcard";
#endif

    //*
    if(m_pAudioInput && m_pCall
       && (pAudioChannel->openMode() & QIODevice::WriteOnly))
    {
        LOG_MODEL_DEBUG("Video", "m_pAudioInput->start;threadid:0x%x", QThread::currentThreadId());

        //m_pAudioInput->start(pAudioChannel);
        m_AudioRecordInput.open(QIODevice::WriteOnly, pAudioChannel, NULL, szRecordFile + "/savein.raw");
        m_pAudioInput->start(&m_AudioRecordInput);
    }

    if(m_pAudioOutput && m_pCall
        && (pAudioChannel->openMode() & QIODevice::ReadOnly))
    {
        LOG_MODEL_DEBUG("Video", "m_pAudioOutput->start");

        //push模式
        //m_pAudioOutput->start(pAudioChannel);
        
        //push模式
        //m_AudioRecordOutput.open(QIODevice::ReadWrite, pAudioChannel, NULL, szRecordFile + "/saveout.raw");
        //m_pAudioOutput->start(&m_AudioRecordOutput);
        
        //pull模式
        QIODevice* pOut = m_pAudioOutput->start();
        m_AudioRecordOutput.open(QIODevice::ReadWrite, pAudioChannel, pOut, szRecordFile + "/saveout.raw");
        
    }//*/

    return nRet;
}

//停止设备，并删除对象
int CFrmVideo::StopAudioDevice()
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

    m_AudioRecordInput.close();
    m_AudioRecordOutput.close();

    return 0;
}

//发送捕获的视频帧 
void CFrmVideo::slotCaptureFrame(const QXmppVideoFrame &frame)
{
    if(!m_pCall)
    {
        LOG_MODEL_DEBUG("Video", "m_pCall is NULL");
        return;
    }

    QXmppRtpVideoChannel *pChannel = m_pCall->videoChannel();
    if(!pChannel || !(pChannel->openMode() & QIODevice::WriteOnly))
    {
        LOG_MODEL_DEBUG("Video", "m_pCall->videoChannel() is null or openMode isn't write mode");
        return;
    }

    pChannel->writeFrame(frame);
}

//接收网络视频帧 
void CFrmVideo::slotUpdateReciverVideo()
{
    if(!m_pCall)
        return;

    QXmppRtpVideoChannel *pChannel = m_pCall->videoChannel();
    if(!pChannel)
        return;

    m_inFrames << pChannel->readFrames();
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("Video", "recive video frames:%d", m_inFrames.size());
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("Video", "preTime:%s, currTime:%s, space:%d",
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
    if(!m_inFrames.isEmpty())
    {
        m_RemotePlayer.slotPresent(*m_inFrames.begin());
        m_inFrames.clear();
    }
//    foreach(QXmppVideoFrame frame, m_inFrames)
//    {
//        m_RemotePlayer.slotPresent(frame);
//    }
}

void CFrmVideo::PlayCallSound()
{
    QString file;
    if(m_bCall)
        file = ":/sound/Call";
    else
        file = ":/sound/Receive";

    if(m_pCallSound)
    {
        m_pCallSound->stop();
        delete m_pCallSound;
    }
    m_pCallSound = new QSound(file);
    if(m_pCallSound)
    {
        m_pCallSound->setLoops(100);
        m_pCallSound->play();
    }
}

void CFrmVideo::StopCallSound()
{
    if(m_pCallSound)
    {
        m_pCallSound->stop();
        delete m_pCallSound;
        m_pCallSound = NULL;
    }
}
