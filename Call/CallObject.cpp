#include "CallObject.h"
#include "Global/Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"

CCallObject::CCallObject(const QString &szId, bool bVideo, QObject *parent) :
    QObject(parent)
{
    m_szId = szId;
    m_Direction = IncomingDirection;
    m_bVideo = bVideo;
    m_pSound = NULL;
    m_pFrmVideo = NULL;
    
    slotChanageState(CallState);
}

CCallObject::~CCallObject()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::~CCallObject.id:%d", qPrintable(m_szId));
    StopCallSound();
    CloseVideoWindow();
}

int CCallObject::Call()
{
    SetDirection(OutgoingDirection);
    return 0;
}

QString CCallObject::GetId()
{
    Q_ASSERT(!m_szId.isEmpty());//请在派生类构造函数中设置此值  
    return m_szId;
}

int CCallObject::SetId(const QString szId)
{
    m_szId = szId;
    return 0;
}

int CCallObject::SetDirection(Direction d)
{
    m_Direction = d;
    return 0;
}

CCallObject::State CCallObject::GetState()
{
    return m_State;
}

CCallObject::Direction CCallObject::GetDirection()
{
    return m_Direction;
}

bool CCallObject::IsVideo()
{
    return m_bVideo;
}

//播放铃音,系统会启用单独的线程进行播放  
void CCallObject::PlayCallSound()
{
    if(IsMonitor())
        return;
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::PlayCallSound");
    QString file;
    if(GetDirection() == CCallObject::OutgoingDirection)
        file = ":/sound/Call";
    else
        file = ":/sound/Receive";

    StopCallSound();
    m_pSound = new QSound(file);
    if(m_pSound)
    {
        m_pSound->setLoops(100);
        m_pSound->play();
    }
}

void CCallObject::StopCallSound()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::StopCallSound");
    if(m_pSound)
    {
        m_pSound->stop();
        delete m_pSound;
        m_pSound = NULL;
    }
}

int CCallObject::OpenVideoWindow()
{
    if(IsMonitor())
        return 0;

    //打开显示对话框  
    if(m_pFrmVideo)
    {
        LOG_MODEL_ERROR("CCallObject", "Video window is opened");
        Q_ASSERT(false);     
        return 0;
    }
    m_pFrmVideo = new CFrmVideo();
    if(!m_pFrmVideo)
    {
        return -1;
    }
    CTool::EnableWake();
    
    //窗口关闭时会自己释放内存  
    m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);
    bool check = connect(m_pFrmVideo, SIGNAL(destroyed()),
                         SLOT(slotFrmVideoClose()));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigRenderLocale(QImage)),
                    m_pFrmVideo, SLOT(slotDisplayLocaleVideo(QImage)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigRenderRemote(QImage)),
                    m_pFrmVideo, SLOT(slotDisplayRemoteVideo(QImage)));
    Q_ASSERT(check);
    m_pFrmVideo->show();
    m_pFrmVideo->activateWindow();

    return 0;
}

int CCallObject::CloseVideoWindow()
{
    if(m_pFrmVideo)
    {
        this->disconnect(m_pFrmVideo);
        //因为在OpenVideoWindow设置了窗口关闭时会自己释放内存  
        //m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);
        //所以这里不需要释放内存  
        m_pFrmVideo->close();
        m_pFrmVideo = NULL;
    }
    return 0;
}

void CCallObject::slotFrmVideoClose()
{
    if(m_pFrmVideo)
    {
        m_pFrmVideo = NULL;
        this->Stop();
    }
}

void CCallObject::slotChanageState(CCallObject::State state)
{
    m_State = state;
    emit sigUpdate();
    switch(m_State)
    {
    case CallState:
        PlayCallSound();
        OpenVideoWindow();
        break;
    case ConnectingState:
        StopCallSound();
        break;
    case DisconnectingState:
        break;
    case FinishedState:
        StopCallSound();
        CloseVideoWindow();
        emit sigFinished(this);
        break;
    default:
        break;        
    }

    return;
}

bool CCallObject::IsMonitor()
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(this->GetId());
    if(roster.isNull())
    {
        LOG_MODEL_DEBUG("CCallObjectQXmpp", "roster is null");
        return false;
    }
    if(GetDirection() == CCallObject::IncomingDirection
            && roster->GetInfo()->GetIsMonitor()
            && CGlobal::Instance()->GetIsMonitor())
    {
        return true;
    }
    return false;
}

