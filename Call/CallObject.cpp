#include "CallObject.h"
#include "Global/Global.h"

CCallObject::CCallObject(bool bVideo, QObject *parent) :
    QObject(parent)
{
    m_Direction = IncomingDirection;
    m_bVideo = bVideo;
    m_pSound = NULL;
}

CCallObject::~CCallObject()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::~CCallObject.id:%d", qPrintable(m_szId));
    StopCallSound();
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

int CCallObject::Accept()
{
    LOG_MODEL_WARNING("CCallObject", "The CCallObject::Accept function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CCallObject::Cancel()
{
    LOG_MODEL_WARNING("CCallObject", "The CCallObject::Cancel function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

CCallObject::State CCallObject::GetState()
{
    LOG_MODEL_WARNING("CCallObject", "The CCallObject::GetState function must be implemented by derived classes");
    Q_ASSERT(false);
    return ConnectingState;
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

    if(m_pSound)
    {
        m_pSound->stop();
        delete m_pSound;
    }
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
