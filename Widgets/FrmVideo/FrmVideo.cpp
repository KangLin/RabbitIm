#include "FrmVideo.h"
#include "ui_FrmVideo.h"
#include <QHostAddress>
#include <QMessageBox>
#include "../FrmUserList/Roster.h"
#include "../../Global.h"
#include "qxmpp/QXmppRtpChannel.h"

extern CGlobal g_Global;

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmVideo)
{
    qDebug("CFrmVideo::CFrmVideo");
    ui->setupUi(this);
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
        m_pClient->m_CallManager.disconnect(this);
        m_pClient->disconnect(this);
    }//*/

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

    this->setWindowTitle(tr("Be ringing ") + QXmppUtils::jidToBareJid(jid));

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
        pCall->accept();
        this->setWindowTitle(tr("Be connecting ") + QXmppUtils::jidToBareJid(m_pCall->jid()));
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
        this->setWindowTitle(tr("Be ringing ") + QXmppUtils::jidToBareJid(m_pCall->jid()));
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

//通道建立连接
void CFrmVideo::connected()
{
    qDebug("CFrmVideo::connected");

    this->setWindowTitle(tr("Be talking ") + QXmppUtils::jidToBareJid(m_pCall->jid()));

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
    }
}

//音频模式改变
void CFrmVideo::audioModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::audioModeChanged:%x", mode);
    if((QIODevice::ReadOnly & mode) && m_pAudioOutput && m_pCall)
    {
        m_pAudioInput->start(m_pCall->audioChannel());
    }

    if((QIODevice::WriteOnly & mode) && m_pAudioOutput && m_pCall)
        m_pAudioOutput->start(m_pCall->audioChannel());
}

//视频模式改变
void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::videoModeChanged:%x", mode);
}

//呼叫结束时触发
void CFrmVideo::finished()
{
    qDebug("CFrmVideo::finished");

    StopDevice();

    if(m_pCall)
    {
        QString szMsg = tr("Close the connection with ") + QXmppUtils::jidToBareJid(m_pCall->jid());

        m_pCall->disconnect(this);//删除所有连接
        m_pCall->deleteLater();//TODO:是否需要应用程序释放内存？到QXmppCall析构函数中打日志
        m_pCall = NULL;

        this->setWindowTitle(szMsg);
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
