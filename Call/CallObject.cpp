#include "CallObject.h"
#include "Global/Global.h"

CCallObject::CCallObject(QObject *parent) :
    QObject(parent)
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

int CCallObject::GetTotalTime()
{
    return m_nTotal;
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
