#include "CallObject.h"
#include "Global/Global.h"

CCallObject::CCallObject(QObject *parent) :
    QObject(parent)
{
    m_Direction = IncomingDirection;
}

CCallObject::~CCallObject()
{
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

//播放铃音,系统会启用单独的线程进行播放  
void CCallObject::PlayCallSound()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::PlayCallSound");
    QString file;
    if(GetDirection() == CCallObject::OutgoingDirection)
        file = ":/sound/Call";
    else
        file = ":/sound/Receive";

    if(!m_Sound.isNull())
    {
        m_Sound->stop();
        m_Sound.clear();
    }
    QSharedPointer<QSound> sound(new QSound(file));
    m_Sound = sound;
    if(!m_Sound.isNull())
    {
        m_Sound->setLoops(100);
        m_Sound->play();
    }
}

void CCallObject::StopCallSound()
{
    LOG_MODEL_DEBUG("CCallObject", "CCallObject::StopCallSound");
    if(!m_Sound.isNull())
    {
        m_Sound->stop();
        m_Sound.clear();
    }
}
