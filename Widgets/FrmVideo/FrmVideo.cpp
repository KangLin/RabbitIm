#include "FrmVideo.h"
#include "ui_FrmVideo.h"
#include <QHostAddress>
#include <QMessageBox>
#include "../FrmUserList/Roster.h"

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmVideo)
{
    ui->setupUi(this);
    m_pCall = NULL;
    m_pClient = NULL;
}

CFrmVideo::~CFrmVideo()
{
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

    QHostAddress host = QHostAddress("183.62.225.76");
    m_pClient->m_CallManager.setStunServer(host);
    m_pClient->m_CallManager.setTurnServer(host, 13478);
    m_pClient->m_CallManager.setTurnUser("foo");
    m_pClient->m_CallManager.setTurnPassword("bar");
    m_pCall = m_pClient->m_CallManager.call(jid);

    return 0;
}

void CFrmVideo::callReceived(QXmppCall *pCall)
{
    qDebug("CFrmVideo::callReceived");
    m_pCall = pCall;
    QHostAddress host = QHostAddress("183.62.225.76");
    m_pClient->m_CallManager.setStunServer(host);
    m_pClient->m_CallManager.setTurnServer(host, 13478);
    m_pClient->m_CallManager.setTurnUser("foo");
    m_pClient->m_CallManager.setTurnPassword("bar");

    QMessageBox msg(QMessageBox::Question,
                    tr("Call"),
                    tr("User %s call").arg(pCall->jid()),
                    QMessageBox::Yes | QMessageBox::No);
    if(QMessageBox::Yes == msg.exec())
        pCall->accept();
    else
        pCall->hangup();
}

void CFrmVideo::callStarted(QXmppCall *call)
{
    qDebug("CFrmVideo::callStarted");
}

void CFrmVideo::connected()
{
    qDebug("CFrmVideo::connected");
}

void CFrmVideo::finished()
{
    qDebug("CFrmVideo::finished");
}

void CFrmVideo::ringing()
{
    qDebug("CFrmVideo::ringing");
}

void CFrmVideo::stateChanged(QXmppCall::State state)
{
    qDebug("CFrmVideo::stateChanged:%d", state);
}

void CFrmVideo::audioModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::audioModeChanged:%x", mode);
}

void CFrmVideo::videoModeChanged(QIODevice::OpenMode mode)
{
    qDebug("CFrmVideo::videoModeChanged:%x", mode);
}
