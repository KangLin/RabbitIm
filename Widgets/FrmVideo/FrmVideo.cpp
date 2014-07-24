/*
 * 视频显示窗体
 * 作者：康林
 * 
 * 注意：本对象是一个单例对象
 */

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
#include <QCameraInfo>
#include "../FrmUserList/Roster.h"
#include "../../Global.h"
#include "qxmpp/QXmppRtpChannel.h"

extern CGlobal g_Global;

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    m_RemotePlayer(this),
    m_LocalePlayer(this),
    m_Camera(this),
    ui(new Ui::CFrmVideo)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::CFrmVideo");
    ui->setupUi(this);

    /*设置提示文本颜色 
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    ui->lbPrompt->setPalette(pe);//*/

    m_pCall = NULL;
    m_pClient = NULL;
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
    m_pCallSound = NULL;
    
    InitCamera();

    ShowWdgInfo(false);
    //当tracking为off时，只有当一个鼠标键按下时，才会有mouseEvent事件。 
    //当tracking为on时，没鼠标键按下，也会有mouseEvent事件 
    this->setMouseTracking(true);

    m_VideoThread.setObjectName("VideoThread");
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
        m_pClient->m_CallManager.disconnect(this);
        m_pClient->disconnect(this);
    }

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
        if(pClient)
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
    check = connect(&m_Camera, SIGNAL(sigCaptureFrame(const QVideoFrame&)),
                    &m_LocalePlayer, SLOT(slotPresent(const QVideoFrame&)));
    Q_ASSERT(check);

    //连接到网络发送 
    check = connect(&m_Camera, SIGNAL(sigConvertedToYUYVFrame(const QXmppVideoFrame&)),
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
            LOG_MODEL_DEBUG("Video","nHeight:%d, nWidth:%d", nHeight, nWidth);
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
    /*QBrush bg(Qt::darkBlue);
    painter.setBrush(bg);
    painter.drawRect(rect());//*/
}

//当tracking为off时，只有当一个鼠标键按下时，才会有mouseEvent事件。 
//当tracking为on时，没鼠标键按下，也会有mouseEvent事件 
void CFrmVideo::mouseMoveEvent(QMouseEvent *event)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::mouseMoveEvent");
    Q_UNUSED(event);
    //ShowToolBar(true);
}

int CFrmVideo::ShowWdgInfo(bool bShow)
{
    if(bShow)
    {
        ui->ToolBar->show();
        ui->wdgInfo->show();
        ui->pbOK->setEnabled(true);
    }
    else
    {
        ui->ToolBar->hide();
        ui->wdgInfo->hide();
        ui->pbOK->setEnabled(false);
    }
    return 0;
}

int CFrmVideo::ShowToolBar(bool bShow)
{
    if(bShow)
    {
        ui->ToolBar->show();
        ui->wdgInfo->show();
    }
    else
    {
        ui->wdgInfo->hide();
        ui->ToolBar->hide();
    }
    return 0;
}

int CFrmVideo::ConnectionCallSlot(QXmppCall *pCall)
{
    //只有主叫方才有的事件 
    bool check = connect(pCall, SIGNAL(ringing()),
                    SLOT(ringing()));
    Q_ASSERT(check);
    
    //以下是双方都有的事件 
    check = connect(pCall, SIGNAL(connected()),
                         SLOT(connected()));
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

    check = connect(pCall, SIGNAL(finished()),
                    SLOT(finished()));
    Q_ASSERT(check);

    if(check)
        return 0;

    return -1;
}

//主动发起呼叫  
int CFrmVideo::Call(QString jid)
{
    //如果已经有调用，则停止并释放  
    if(m_pCall)
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Call"),
                        tr("Being talk with %1 , Do you stop it?").arg(QXmppUtils::jidToUser(m_pCall->jid())),
                        QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == msg.exec())
        {
            m_pCall->hangup();
        }
        //不能同时有两个呼叫，因为呼叫挂断需要时间，所以直接返加，然后再由用户重新发起呼叫。  
        return -1;
    }

    m_szRemoteJID = jid;//保存被叫用户jid  

    QString szText = tr("%1 is ringing").arg(QXmppUtils::jidToUser(jid));
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);

    m_pClient->m_CallManager.setStunServer(
                QHostAddress(CGlobal::Instance()->GetStunServer()),
                CGlobal::Instance()->GetStunServerPort()
                );
    m_pClient->m_CallManager.setTurnServer(
                QHostAddress(CGlobal::Instance()->GetStunServer()),
                CGlobal::Instance()->GetTurnServerPort()
                );
    m_pClient->m_CallManager.setTurnUser(
                CGlobal::Instance()->GetTurnServerUser()
                );
    m_pClient->m_CallManager.setTurnPassword(
                CGlobal::Instance()->GetTurnServerPassword()
                );
    
    //返回QXmppCall的引用,这个会由QXmppCallManager管理.用户层不要释放此指针  
    QXmppCall* m_pCall = m_pClient->m_CallManager.call(jid);
    if(NULL == m_pCall)
    {
        LOG_MODEL_ERROR("Video",  "call %s fail", qPrintable(jid));
        return -2;
    }

    ConnectionCallSlot(m_pCall);

    //播放铃音,非阻塞式播放  
    PlayCallSound(m_pCall);
    
    ShowWdgInfo(true);
    ui->pbOK->setEnabled(false);
    //显示本窗口  
    this->show();

    return 0;
}

//接收呼叫（只有被叫方才有） 
void CFrmVideo::callReceived(QXmppCall *pCall)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::callReceived:%x", pCall);
    if(m_pCall)
    {
        LOG_MODEL_ERROR("Video", 
                        qPrintable(CGlobal::Instance()->GetBareJid() 
                         + " talking with "
                         + QXmppUtils::jidToBareJid(m_pCall->jid())
                         + "call, don't accpect new call."));
        pCall->hangup();//TODO：一个用户不能同时与两个用户建立呼叫,所以自动挂断新的呼叫，是否要加一个忙参数，否则呼叫方不知道原因。  
        //TODO:是否需要提示给用户  
        return;
    }

    m_pCall = pCall;
    ConnectionCallSlot(pCall);
    m_szRemoteJID = pCall->jid();
    m_pClient->m_CallManager.setStunServer(
                QHostAddress(CGlobal::Instance()->GetStunServer()),
                CGlobal::Instance()->GetStunServerPort()
                );
    m_pClient->m_CallManager.setTurnServer(
                QHostAddress(CGlobal::Instance()->GetTurnServer()),
                CGlobal::Instance()->GetTurnServerPort()
                );
    m_pClient->m_CallManager.setTurnUser(
                CGlobal::Instance()->GetTurnServerUser()
                );
    m_pClient->m_CallManager.setTurnPassword(
                CGlobal::Instance()->GetTurnServerPassword()
                );

    //播放铃音,非阻塞式播放  
    PlayCallSound(pCall);

    QString szMsg = tr("%1 is calling ").arg(QXmppUtils::jidToUser(pCall->jid()));
    ui->lbPrompt->setText(szMsg);
    this->setWindowTitle(szMsg);
    //TODO:增加判断自动接收呼叫用户  
    //非模式对话框,超时自动挂机  
    ShowWdgInfo(true);
    this->show();
    return;
    //接收或挂机放在按钮响应事件(on_pbOk_clicked、on_pbCancel_clicked)中处理  
}

//呼叫开始(主叫与被叫都有） 
void CFrmVideo::callStarted(QXmppCall *pCall)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::callStarted:%x", pCall);
    //保存当前call指针
    m_pCall = pCall;
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
    QString szState;
    switch (state)
    {
    case QXmppCall::ConnectingState:
        szState = tr("State: Connecting Call");
        break;
    case QXmppCall::ActiveState:
        szState = tr("State: Active Call");
        break;
    case QXmppCall::DisconnectingState:
        szState = tr("State: Disconnecting Call");
        break;
    case QXmppCall::FinishedState:
        szState = tr("State: Finished Call");
        break;
    default:
        break;
    }

    LOG_MODEL_DEBUG("Video", "CFrmVideo::stateChanged:%s", qPrintable(szState));
    ui->lbPrompt->setText(szState);
}

//会话建立后触发  
void CFrmVideo::connected()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::connected");

    //停止铃音  
    StopCallSound();
    ShowWdgInfo(false);
    
    QString szText = tr("I am talking with %1 call").arg(QXmppUtils::jidToUser(m_pCall->jid()));
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);

    //初始始化音频设备  
    StartAudioDevice();

    //初始化视频设备，并开始视频  
    StartVideo();
}

//音频模式改变  
void CFrmVideo::audioModeChanged(QIODevice::OpenMode mode)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::audioModeChanged:%x", mode);

    if(!(m_pCall && m_pAudioInput && m_pAudioOutput))
        return;

    if(QIODevice::WriteOnly & mode)
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioInput CFrmVideo::audioModeChanged:%xm_pAudioInput->start()", mode);
        m_pAudioInput->start(m_pCall->audioChannel());
    }
    else 
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioInput CFrmVideo::audioModeChanged:%x---m_pAudioInput->stop()", mode);
        m_pAudioInput->stop();
    }

    if(QIODevice::ReadOnly & mode)
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioOutput CFrmVideo::audioModeChanged:%x-----------m_pAudioOutput->start()", mode);
        m_pAudioOutput->start(m_pCall->audioChannel());
    }
    else
    {
        LOG_MODEL_DEBUG("Video", "OpenAudioOutput CFrmVideo::audioModeChanged:%x-----------m_pAudioOutput->stop()", mode);
        m_pAudioOutput->stop();
    }
}

//视频模式改变  
void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    if(!(m_pCall))
        return;

    LOG_MODEL_DEBUG("Video", "CFrmVideo::videoModeChanged:%x", mode);
    if(QIODevice::ReadOnly & mode)
    {
        //int nInterval = (double)1000 / m_pCall->videoChannel()->decoderFormat().frameRate();
        m_VideoPlayTimer.start(10); //TODO:按 10ms 取视频帧，目前没有做缓存抖动处理  
    }
    else
        m_VideoPlayTimer.stop();
}

//呼叫结束时触发  
void CFrmVideo::finished()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::finished");

    //停止播放铃音  
    StopCallSound();

    //停止播放设备  
    StopAudioDevice();

    if(m_pCall)
    {
        QString szMsg = tr("Do you call %1 ?").arg(QXmppUtils::jidToUser(m_pCall->jid()));
        this->setWindowTitle(szMsg);
        ui->lbPrompt->setText(szMsg);

        StopVideo();
        m_VideoPlayTimer.stop();

        m_pCall->disconnect(this);//删除所有连接  
        m_pCall = NULL;//m_pCall只是个引用，对象由QXmppCallManager管理.用户层不要释放此指针  
    }
    ShowWdgInfo(true);
}

//播放铃音,系统会启用单独的线程进行播放  
void CFrmVideo::PlayCallSound(QXmppCall* pCall)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::PlayCallSound");
    QString file;
    if(pCall->direction() == QXmppCall::OutgoingDirection)
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
    LOG_MODEL_DEBUG("Video", "CFrmVideo::StopCallSound");
    if(m_pCallSound)
    {
        m_pCallSound->stop();
        delete m_pCallSound;
        m_pCallSound = NULL;
    }
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
#ifdef DEBUG
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
#endif
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
#ifdef DEBUG
    ShowAudioDeviceSupportCodec(infoAudioInput, "input device");
#endif
    QAudioDeviceInfo infoAudioOutput(QAudioDeviceInfo::defaultOutputDevice());
    if (!infoAudioOutput.isFormatSupported(outFormat)) {
        LOG_MODEL_WARNING("Video", "Default audio output format not supported - trying to use nearest");
        outFormat = infoAudioOutput.nearestFormat(outFormat);
    }
#ifdef DEBUG
    ShowAudioDeviceSupportCodec(infoAudioOutput, "output device");
#endif
    m_pAudioInput = new QAudioInput(infoAudioInput, inFormat, this);
    if(!m_pAudioInput)
        LOG_MODEL_ERROR("Video", "Create QAudioInput device instance fail.");
    m_pAudioOutput = new QAudioOutput(infoAudioOutput, outFormat, this);
    if(!m_pAudioOutput)
        LOG_MODEL_ERROR("Video", "Create QAudioOutput device instance fail.");

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

    return 0;
}

#ifdef ANDROID
QCamera::Position CFrmVideo::GetCameraPostion()
{
    return m_Camera.GetCameraPoistion();
}
#endif

int CFrmVideo::InitCamera()
{
    QList<QString> dev = m_Camera.GetAvailableDevices();
    QList<QString>::iterator it;
    for (it = dev.begin(); it != dev.end(); it++)
    {
        ui->cmbCamera->addItem(*it);
    }
    ui->cmbCamera->setCurrentIndex(m_Camera.GetDeviceIndex());
    return 0;
}

int CFrmVideo::StartVideo()
{
    if(!m_pCall)
    {
        LOG_MODEL_ERROR("Video", "CFrmVideo::OpenCamera m_pCall is null");
        return -1;
    }

    LOG_MODEL_DEBUG("Video", "CFrmVideo::StartVideo");

    m_Camera.Start();

    // 改变默认的视频编码格式  
    //m_pCall->videoChannel()->setEncoderFormat(videoFormat);
    //开始视频，只需要呼叫方调用startVideo  
    if(m_pCall->direction() == QXmppCall::OutgoingDirection)
        m_pCall->startVideo();

    ui->lbPrompt->hide();
    resizeEvent(NULL); //通知窗体按视频大小调整显示区域  
    m_RemotePlayer.show();
    m_RemotePlayer.setWindowTitle(QXmppUtils::jidToUser(m_pCall->jid()));

    m_LocalePlayer.setWindowTitle(CGlobal::Instance()->GetName());
    m_LocalePlayer.raise();//提升到父窗口中栈的顶部  
    m_LocalePlayer.show();
    m_LocalePlayer.activateWindow();

    return 0;
}

int CFrmVideo::StopVideo()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::StopVideo");
    m_Camera.Stop();

    m_LocalePlayer.close();
    m_RemotePlayer.close();
    
    if(m_pCall)
        if(m_pCall->direction() == QXmppCall::OutgoingDirection)
            m_pCall->stopVideo();

    ui->lbPrompt->show();
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

    /*if(!m_inFrames.isEmpty())
    {
        m_RemotePlayer.slotPresent(*m_inFrames.begin());
        m_inFrames.pop_front();
        m_inFrames.clear();
    }*/
    foreach(QXmppVideoFrame frame, m_inFrames)
    {
        m_RemotePlayer.slotPresent(frame);
    }
    m_inFrames.clear();
}

void CFrmVideo::on_pbOK_clicked()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_pbOK_clicked");
    if(!m_pCall)
    {
        LOG_MODEL_DEBUG("Video", "CFrmVideo::on_pbOK_clicked m_pCall is null, Recall %s", qPrintable(m_szRemoteJID));
        Call(m_szRemoteJID);
        return;
    }

    QString szText = tr("Be connecting %1").arg(QXmppUtils::jidToUser(m_pCall->jid()));
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);
    ui->ToolBar->hide();

    if(m_pCall->direction() == QXmppCall::IncomingDirection)
    {
        m_pCall->accept();
    }
}

void CFrmVideo::on_pbCancel_clicked()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_pbCancel_clicked");
    //停止播放铃音  
    StopCallSound();

    if(!m_pCall)
    {
        LOG_MODEL_DEBUG("Video", "Close windows");
        this->close();
        return;
    }
    //LOG_MODEL_DEBUG("Video", "m_pCall->direction():%d", m_pCall->direction());
    m_pCall->hangup();
}

void CFrmVideo::on_cmbCamera_currentIndexChanged(int index)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_cmbCamera_currentIndexChanged");

    m_Camera.SetDeviceIndex(index);
}
