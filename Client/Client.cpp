#include "Client.h"
#include "Global/Global.h"

CClient::CClient(QObject *parent) :
    QObject(parent)
{
}

CClient::~CClient()
{
}

int CClient::Login(const QString &szUserName, const QString &szPassword, CUserInfo::USER_INFO_STATUS status)
{
    LOG_MODEL_WARNING("CClient", "The CClient::Login function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::Logout()
{
    LOG_MODEL_WARNING("CClient", "The CClient::Logout function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RequestUserInfoLocale()
{
    LOG_MODEL_WARNING("CClient", "The CClient::RequestUserInfoLocale function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RequestUserInfoRoster(const QString &szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_WARNING("CClient", "The CClient::RequestUserInfoRoster function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::setClientStatus(CUserInfo::USER_INFO_STATUS status)
{
    Q_UNUSED(status);
    LOG_MODEL_WARNING("CClient", "The CClient::setClientStatus function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RosterSubscribe(const QString &szId, const QString &szName, const QSet<QString> &groups)
{
    Q_UNUSED(szId);
    Q_UNUSED(szName);
    Q_UNUSED(groups);
    LOG_MODEL_WARNING("CClient", "The CClient::RosterSubscribe function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RosterUnsubscribe(const QString &szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_WARNING("CClient", "The CClient::RosterUnsubscribe function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}
