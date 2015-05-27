#include "ManageCallXmpp.h"
#include "Global/Global.h"
#include "Client/ClientXmpp.h"
#include "UserInfo/UserInfoXmpp.h"
#include "CallObjectQXmpp.h"

CManageCallXmpp::CManageCallXmpp(QObject *parent) : CManageCall(parent)
{}

CManageCallXmpp::~CManageCallXmpp()
{}

int CManageCallXmpp::Init(const QString &szId)
{
    int nRet = 0;
    nRet = CManageCall::Init(szId);
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

int CManageCallXmpp::Clean()
{
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

    return CManageCall::Clean();
}

int CManageCallXmpp::OnCall(const QString &szId, bool bVideo)
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

    Q_ASSERT(pCallManager);

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
    QSharedPointer<CCallObject> call(new CCallObjectQXmpp(pCall, bVideo));
    if(call.isNull())
    {
        LOG_MODEL_DEBUG("CManageCall", "CManageCall::CallVideo fail");
        return -3;
    }
    m_Call.insert(szId, call);

    //关联信号  
    bool check = false;
    check = connect(call.data(), SIGNAL(sigFinished(CCallObject*)),
                         SLOT(slotCallFinished(CCallObject*)));
    Q_ASSERT(check);

    return 0;
}

void CManageCallXmpp::slotReceived(QXmppCall *pCall)
{
    //TODO:这里实现并不知道是包含视频  
    QSharedPointer<CCallObject> call(new CCallObjectQXmpp(pCall));
    emit slotCallReceived(call);
    return;
}
