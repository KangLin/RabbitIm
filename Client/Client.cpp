#include "Client.h"
#include "Global/Global.h"

CClient::CClient(QObject *parent) :
    QObject(parent)
{
}

CClient::~CClient()
{
}

int CClient::Register(const QString &szId, const QString &szName, const QString &szPassword, const QString &szEmail, const QString &szDescript)
{
    Q_UNUSED(szId);
    Q_UNUSED(szName);
    Q_UNUSED(szPassword);
    Q_UNUSED(szEmail);
    Q_UNUSED(szDescript);
    LOG_MODEL_WARNING("CClient", "The CClient::Register function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
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

int CClient::RosterAdd(const QString &szId, SUBSCRIBE_TYPE type, const QString &szName, const QSet<QString> &groups)
{
    Q_UNUSED(szId);
    Q_UNUSED(szName);
    Q_UNUSED(groups);
    Q_UNUSED(type);
    LOG_MODEL_WARNING("CClient", "The CClient::RosterAdd function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RosterRemove(const QString &szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_WARNING("CClient", "The CClient::RosterRemove function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::RosterRename(const QString &szId, const QString &szName)
{
    Q_UNUSED(szId);
    LOG_MODEL_WARNING("CClient", "The CClient::RosterRename function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::SendMessage(const QString &szId, const QString &szMsg)
{
    Q_UNUSED(szId);
    Q_UNUSED(szMsg);
    LOG_MODEL_WARNING("CClient", "The CClient::SendMessage function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CClient::setlocaleUserInfo(QSharedPointer<CUserInfo> userInfo)
{
    Q_UNUSED(userInfo);
    LOG_MODEL_WARNING("CClient", "The CClient::setlocaleUserInfo function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

QSharedPointer<CFileTransfer> CClient::SendFile(const QString szId, const QString &szFile, const QString &szDescription)
{
    Q_UNUSED(szId);
    Q_UNUSED(szFile);
    Q_UNUSED(szDescription);
    LOG_MODEL_WARNING("CClient", "The CClient::SendFile function must be implemented by derived classes");
    Q_ASSERT(false);
    return QSharedPointer<CFileTransfer>();
}

QSharedPointer<CCallObject> CClient::CallVideo(const QString szId)
{
    Q_UNUSED(szId);
    LOG_MODEL_WARNING("CClient", "The CClient::CallVideo function must be implemented by derived classes");
    Q_ASSERT(false);
    return QSharedPointer<CCallObject>();
}
