#include "ManageCallXmpp.h"
#include "Global/Global.h"
#include "ClientXmpp.h"
#include "UserInfoXmpp.h"
#include "CallObjectQXmpp.h"

CManageCallXmpp::CManageCallXmpp(QObject *parent) : CManageCall(parent)
{}

CManageCallXmpp::~CManageCallXmpp()
{}

int CManageCallXmpp::LoginInit(const QString &szId)
{
    int nRet = 0;
    nRet = CManageCall::LoginInit(szId);
    if(nRet)
        return nRet;

    CClientXmpp* pClient = (CClientXmpp*)GETMANAGER->GetClient().data();
    if(!pClient)
    {
        LOG_MODEL_ERROR("Call", "pClient is null");
        return -1;
    }

    QXmppCallManager* pCallManager = pClient->m_Client.findExtension<QXmppCallManager>();
    if(!pCallManager)
    {
        pClient->m_Client.addExtension(new QXmppCallManager);
        pCallManager = pClient->m_Client.findExtension<QXmppCallManager>();
    }
    Q_ASSERT(pCallManager);
    //设置ice服务器参数  
    pCallManager->setStunServer(
                QHostAddress(CGlobal::Instance()->GetStunServer()),
                CGlobal::Instance()->GetStunServerPort()
                );
    pCallManager->setTurnServer(
                QHostAddress(CGlobal::Instance()->GetTurnServer()),
                CGlobal::Instance()->GetTurnServerPort()
                );
    pCallManager->setTurnUser(
                CGlobal::Instance()->GetTurnServerUser()
                );
    pCallManager->setTurnPassword(
                CGlobal::Instance()->GetTurnServerPassword()
                );

    bool check = false;
    check = connect(pCallManager, SIGNAL(callReceived(QXmppCall*)),
                         SLOT(slotReceived(QXmppCall*)));
    Q_ASSERT(check);

    return 0;
}

int CManageCallXmpp::LogoutClean()
{
    CManageCall::LogoutClean();
    CClientXmpp* pClient = (CClientXmpp*)GETMANAGER->GetClient().data();
    if(!pClient)
    {
        LOG_MODEL_ERROR("Call", "pClient is null");
        return -1;
    }

    QXmppCallManager* pCallManager = pClient->m_Client.findExtension<QXmppCallManager>();
    if(pCallManager)
    {
        bool check = false;
        check = pCallManager->disconnect();
        Q_ASSERT(check);
        pClient->m_Client.removeExtension(pCallManager);
    }

    return 0;
}

int CManageCallXmpp::Call(const QString &szId, bool bVideo)
{
    //检查是否是好友  
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageCall", "Don't get roster:%s", qPrintable(szId));
        return -1;
    }
    CUserInfoXmpp* pInfo = (CUserInfoXmpp*)roster->GetInfo().data();
    if(pInfo->GetResource().isEmpty())
    {
        LOG_MODEL_ERROR("Call", "CClientXmpp::Call the roster resource is null");
        roster->GetMessage()->AddMessage(szId,
                tr("The roster is offline, don't launch a call."), true);
        emit GET_CLIENT->sigMessageUpdate(szId);
        return -2;
    }

    return CManageCall::Call(szId, bVideo);
}

int CManageCallXmpp::OnCall(const QString &szId,
            QSharedPointer<CCallObject> &call, bool bVideo)
{
    CClientXmpp* pClient = (CClientXmpp*)GETMANAGER->GetClient().data();
    if(!pClient)
    {
        LOG_MODEL_ERROR("Call", "pClient is null");
        return -1;
    }

    QXmppCallManager* pCallManager = pClient->m_Client.findExtension<QXmppCallManager>();
    if(!pCallManager)
    {
        LOG_MODEL_ERROR("Call", "pCallManager is null");
        return -2;
    }

    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageCallXmpp", "CManageCallXmpp::OnCall the roster is null");
        return -1;
    }

    //因为 xmpp 协议呼叫需要用户的资源（jid）  
    CUserInfoXmpp* info = (CUserInfoXmpp*)roster->GetInfo().data();
    QXmppCall* pCall = pCallManager->call(info->GetJid());
    //新建呼叫对象,并增加到管理 map 中  
    QSharedPointer<CCallObject> callObject(new CCallObjectQXmpp(pCall, bVideo));
    if(callObject.isNull())
    {
        LOG_MODEL_DEBUG("CManageCall", "CManageCall::CallVideo fail");
        return -3;
    }

    call = callObject;

    return 0;
}

void CManageCallXmpp::slotReceived(QXmppCall *pCall)
{
    //TODO:这里实现并不知道是包含视频  
    QSharedPointer<CCallObject> call(new CCallObjectQXmpp(pCall));
    slotCallReceived(call);
    return;
}
