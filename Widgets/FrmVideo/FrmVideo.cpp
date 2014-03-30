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
    ui->setupUi(this);
    m_pCall = NULL;
    m_pClient = NULL;
    m_pAudioInput = NULL;
    m_pAudioOutput = NULL;
}

CFrmVideo::~CFrmVideo()
{
    finished();

    delete ui;
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
    return 0;
}

int CFrmVideo::Call(QString jid)
{
    //TODO:如果已经有调用，则停止并释放

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

void CFrmVideo::callReceived(QXmppCall *pCall)
{
    qDebug("CFrmVideo::callReceived:%x", pCall);
    m_pCall = pCall;
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

   // pCall->accept();
    QMessageBox msg(QMessageBox::Question,
                    tr("Call"),
                    tr("User ") + pCall->jid() + tr(" calling"),
                    QMessageBox::Yes | QMessageBox::No);
    if(QMessageBox::Yes == msg.exec())
    {
        pCall->accept();
    }
    else
        pCall->hangup();//*/
}

void CFrmVideo::callStarted(QXmppCall *pCall)
{
    qDebug("CFrmVideo::callStarted:%x", pCall);
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

void CFrmVideo::ringing()
{
    qDebug("CFrmVideo::ringing");
}

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

void CFrmVideo::connected()
{
    qDebug("CFrmVideo::connected");
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

        finished();

        m_pAudioInput = new QAudioInput(info, format, this);
        m_pAudioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
    }
}

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

void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::videoModeChanged:%x", mode);
}

void CFrmVideo::finished()
{
    qDebug("CFrmVideo::finished");
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
}
