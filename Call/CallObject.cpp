#include "CallObject.h"
#include "Global/Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"

CCallObject::CCallObject(bool bVideo, QObject *parent) :
    QObject(parent)
{
    m_Direction = IncomingDirection;
    m_bVideo = bVideo;
    m_pSound = NULL;
    m_State = CallState;
    m_pFrmVideo = NULL;
}

CCallObject::~CCallObject()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::~CCallObject.id:%d", qPrintable(m_szId));
    StopCallSound();
    CloseVideoWindow();
}

QString CCallObject::GetId()
{
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
    //打开显示对话框  
    if(m_pFrmVideo)
    {
        m_pFrmVideo->close();
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
    }
    this->Stop();
}
