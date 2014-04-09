#include "../../Tool.h"
#include "FrmVideo.h"
#include "ui_FrmVideo.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QPainter>
#include <QAbstractVideoBuffer>
#include "../FrmUserList/Roster.h"
#include "../../Global.h"
#include "qxmpp/QXmppRtpChannel.h"

extern CGlobal g_Global;

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    //m_LocalePlayer(this),
    //m_RemotePlayer(this),
    ui(new Ui::CFrmVideo)
{
    qDebug("CFrmVideo::CFrmVideo");
    ui->setupUi(this);

    m_bCall = false;
    m_pCall = NULL;
    m_pClient = NULL;
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
}

CFrmVideo::~CFrmVideo()
{
    qDebug("CFrmVideo::~CFrmVideo");

    if(m_pClient)
    {
        //m_pClient->m_CallManager.disconnect(this);
        //m_pClient->disconnect(this);
    }//TODO:释放连接，但会引起core dump*/

    delete ui;

    qDebug("CFrmVideo::~CFrmVideo end");
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

    //关联本地视频头捕获视频帧信号到本地播放视频窗口
    check = connect(&m_CaptureVideoFrame, SIGNAL(CaptureFrame(QVideoFrame)),
                    &m_LocalePlayer, SLOT(present(QVideoFrame)));
    Q_ASSERT(check);

    check = connect(&m_CaptureVideoFrame, SIGNAL(CaptureFrame(QVideoFrame)),
                    SLOT(CaptureFrame(QVideoFrame)));
    Q_ASSERT(check);

    return 0;
}

void CFrmVideo::clientIqReceived(const QXmppIq &iq)
{
    qDebug("CFrmVideo::clientIqReceived:%d", iq.error().condition());
}

void CFrmVideo::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    qDebug("CFrmVideo::closeEvent");
    if(m_pCall)
        m_pCall->hangup();
}

void CFrmVideo::resizeEvent(QResizeEvent *)
{
   m_LocalePlayer.setGeometry(this->geometry());
   m_RemotePlayer.setGeometry(geometry());
}

void CFrmVideo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

//主动发起呼叫
int CFrmVideo::Call(QString jid)
{
    //如果已经有调用，则停止并释放
    if(m_pCall)
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Call"),
                        tr("User ")
                        + QXmppUtils::jidToBareJid(m_pCall->jid())
                        + tr(" is calling, Do you stop it?"),
                        QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == msg.exec())
        {
            m_pCall->hangup();
        }
        else
            return -1;
    }

    QString szText = tr("Be ringing ") + QXmppUtils::jidToBareJid(jid);
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

    m_bCall = true;

    return 0;
}

//接收呼叫
void CFrmVideo::callReceived(QXmppCall *pCall)
{
    qDebug("CFrmVideo::callReceived:%x", pCall);
    if(m_pCall)
        qCritical(qPrintable(tr("Is calling ") + QXmppUtils::jidToBareJid(m_pCall->jid())));

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

    QMessageBox msg(QMessageBox::Question,
                    tr("Call"),
                    tr("User ")
                    + QXmppUtils::jidToBareJid(pCall->jid())
                    + tr(" calling"),
                    QMessageBox::Yes | QMessageBox::No);
    if(QMessageBox::Yes == msg.exec())
    {
        QString szText = tr("Be connecting ") + QXmppUtils::jidToBareJid(m_pCall->jid());
        this->setWindowTitle(szText);
        ui->lbPrompt->setText(szText);
        pCall->accept();
    }
    else
        pCall->hangup();
}

//呼叫开始
void CFrmVideo::callStarted(QXmppCall *pCall)
{
    qDebug("CFrmVideo::callStarted:%x", pCall);
    if(m_pCall != pCall)
        qCritical("CFrmVideo::callStarted m_pCall != pCall");

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

    show();
}

//被叫正在响铃(只有主叫方才有)
void CFrmVideo::ringing()
{
    qDebug("CFrmVideo::ringing");
    if(m_pCall)
    {
        QString szText = tr("Be ringing ") + QXmppUtils::jidToBareJid(m_pCall->jid());
        this->setWindowTitle(szText);
        ui->lbPrompt->setText(szText);
    }
}

//呼叫状态发生改变
void CFrmVideo::stateChanged(QXmppCall::State state)
{
    qDebug("CFrmVideo::stateChanged:%d", state);
}

void ShowAudioDeviceSupportCodec(QAudioDeviceInfo &info)
{
    QStringList codec = info.supportedCodecs();
    QStringList::iterator it;
    QString szCodecs;
    for(it = codec.begin(); it != codec.end(); it++)
    {
        QString s = *it;
        szCodecs += s;
    }
    qDebug("audio device support codec:%s", qPrintable(szCodecs));
}

//会话建立后触发
void CFrmVideo::connected()
{
    qDebug("CFrmVideo::connected");

    QString szText = tr("Be talking ") + QXmppUtils::jidToBareJid(m_pCall->jid());
    this->setWindowTitle(szText);
    ui->lbPrompt->setText(szText);

    QXmppRtpAudioChannel* pAudioChannel = m_pCall->audioChannel();
    if(pAudioChannel)
    {
        QXmppJinglePayloadType AudioPlayLoadType = pAudioChannel->payloadType();
        qDebug("CFrmVideo::connected:audio name:%s;id:%d;channels:%d, clockrate:%d",
               qPrintable(AudioPlayLoadType.name()),
               AudioPlayLoadType.id(),
               AudioPlayLoadType.channels(),
               AudioPlayLoadType.clockrate());

        QAudioFormat format;
        format.setSampleRate(AudioPlayLoadType.clockrate());
        format.setChannelCount(AudioPlayLoadType.channels());
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
        if (!info.isFormatSupported(format)) {
            qWarning() << "Default format not supported - trying to use nearest";
            format = info.nearestFormat(format);
        }

        ShowAudioDeviceSupportCodec(info);

        StopDevice();

        m_pAudioInput = new QAudioInput(info, format, this);
        m_pAudioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);

        if(m_pAudioOutput && m_pCall)
        {
            qDebug("m_pAudioInput->start");
            m_pAudioInput->start(m_pCall->audioChannel());
        }

        if(m_pAudioOutput && m_pCall)
        {
            qDebug("m_pAudioOutput->start");
            m_pAudioOutput->start(m_pCall->audioChannel());
        }
    }

    if(m_bCall)
        m_pCall->startVideo();

    QList<QByteArray> device = QCamera::availableDevices();
    QList<QByteArray>::iterator it;
    for(it = device.begin(); it != device.end(); it++)
    {
        qDebug("Camera:%s", qPrintable(QCamera::deviceDescription(*it)));
    }

    m_Camera.setCaptureMode(QCamera::CaptureVideo);
    m_CaptureVideoFrame.setSource(&m_Camera);

    m_Camera.start();
    m_LocalePlayer.show();
    m_LocalePlayer.activateWindow();
    m_LocalePlayer.setWindowTitle(g_Global.GetName());

}

//音频模式改变
void CFrmVideo::audioModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::audioModeChanged:%x", mode);
    /*if((QIODevice::ReadOnly & mode) && m_pAudioOutput && m_pCall)
    {
        m_pAudioInput->start(m_pCall->audioChannel());
    }

    if((QIODevice::WriteOnly & mode) && m_pAudioOutput && m_pCall)
        m_pAudioOutput->start(m_pCall->audioChannel());//*/
}

//视频模式改变
void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::videoModeChanged:%x", mode);
    if(QIODevice::ReadOnly & mode && m_pCall)
    {
        m_RemotePlayer.show();
        m_RemotePlayer.setWindowTitle(QXmppUtils::jidToUser(m_pCall->jid()));
    }
}

//呼叫结束时触发
void CFrmVideo::finished()
{
    qDebug("CFrmVideo::finished");

    m_bCall = false;
    StopDevice();

    if(m_pCall)
    {
        QString szMsg = tr("Close the connection with ") + QXmppUtils::jidToBareJid(m_pCall->jid());

        m_Camera.stop();
        m_LocalePlayer.close();
        m_RemotePlayer.close();
        m_pCall->stopVideo();

        m_pCall->disconnect(this);//删除所有连接
        m_pCall->deleteLater();//需要应用程序释放内存
        m_pCall = NULL;

        this->setWindowTitle(szMsg);
        ui->lbPrompt->setText(szMsg);
        QMessageBox msg(QMessageBox::Question,
                        tr("Call"),
                        szMsg,
                        QMessageBox::Yes);
        msg.exec();
    }
}

//停止设备，并删除对象
int CFrmVideo::StopDevice()
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

void CFrmVideo::CaptureFrame(const QVideoFrame &frame)
{
    if(!m_pCall)
        return;

    QXmppRtpVideoChannel *pChannel = m_pCall->videoChannel();
    if(!pChannel)
        return;

    //TODO:删除,需要重新做一个接收播放线程或定时器
    QList<QXmppVideoFrame> inFrames = pChannel->readFrames();
    if(!inFrames.isEmpty() && inFrames.begin() != inFrames.end())
    {
        m_RemotePlayer.present(*inFrames.begin());
    }

    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;

    do{
        //转换图片格式
        AVPicture pic;
        int nRet = 0;
        int nWidth = 320, nHeight = 240;
        nRet = CTool::ConvertFormat(inFrame, pic, nWidth, nHeight, AV_PIX_FMT_YUYV422);
        if(nRet)
        {
            qDebug("CTool::ConvertFormat fail");
            break;
        }

        int size = avpicture_get_size(AV_PIX_FMT_YUYV422, nWidth, nHeight);
        QSize sizeFrame(nWidth, nHeight);
        QXmppVideoFrame f(size, sizeFrame, size / nHeight, QXmppVideoFrame::Format_YUYV);
        nRet = avpicture_layout(&pic, AV_PIX_FMT_YUYV422, nWidth, nHeight, f.bits(), size);
        if(nRet > 0)
        {
            pChannel->writeFrame(f);
        }

        avpicture_free(&pic);
    }while(0);

    inFrame.unmap();
}
