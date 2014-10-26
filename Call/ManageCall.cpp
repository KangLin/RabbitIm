#include "ManageCall.h"
#include "CallVideo.h"
#include "Client/Client.h"
#include "Global/Global.h"
#include "CallAction.h"
#include <QMessageBox>

CManageCall::CManageCall(QObject *parent) :
    QObject(parent)
{
}

CManageCall::~CManageCall()
{
}

int CManageCall::Init(QString szId)
{
    bool check = false;
    Q_UNUSED(szId);
    check = connect(GET_CLIENT.data(), SIGNAL(sigCallVideoReceived(QSharedPointer<CCallObject>)),
                    SLOT(slotCallVideoReceived(QSharedPointer<CCallObject>)));
    Q_ASSERT(check);
    return 0;
}

int CManageCall::Clean()
{
    int nRet = 0;
    GET_CLIENT->disconnect(this);
    return nRet;
}

int CManageCall::CallVideo(QString szId)
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageCall", "Don't get roster:%s", qPrintable(szId));
        return -1;
    }
    //检查是否正在视频  
    if(!m_Call.isNull())
    {
        QString szCallId = m_Call->GetId();
        QString szShowName = szCallId;
        QSharedPointer<CUser> callRoster = GLOBAL_USER->GetUserInfoRoster(szCallId);
        if(!callRoster.isNull())
            szShowName = callRoster->GetInfo()->GetShowName();
        
        roster->GetMessage()->AddMessage(szId, "Being talk with" + szShowName + ", please stop it.");
        return -2;
    }

    QSharedPointer<CCallObject> call = GET_CLIENT->CallVideo(szId);
    if(call.isNull())
    {
        LOG_MODEL_DEBUG("CManageCall", "CManageCall::CallVideo fail");
        return -3;
    }
    m_Call = call;
    QSharedPointer<CCallAction> action(new CCallAction(m_Call, szId, QTime::currentTime(), true));
    roster->GetMessage()->AddMessage(action);
    emit GET_CLIENT->sigMessageUpdate(szId);
    return 0;
}

void CManageCall::slotCallVideoReceived(QSharedPointer<CCallObject> call)
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(call->GetId());
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageCall", "Don't get roster:%s", qPrintable(call->GetId()));
        return;
    }
    //检查是否正在视频  
    if(!m_Call.isNull())
    {
        QString szCallId = m_Call->GetId();
        QString szShowName = szCallId;
        QSharedPointer<CUser> callRoster = GLOBAL_USER->GetUserInfoRoster(szCallId);
        if(!callRoster.isNull())
            szShowName = callRoster->GetInfo()->GetShowName();
        roster->GetMessage()->AddMessage(call->GetId(), "Being talk with " + szShowName + ", please stop it.");
        call->Cancel();
        return;
    }
    m_Call = call;
    QSharedPointer<CCallAction> action(new CCallAction(m_Call, m_Call->GetId(), QTime::currentTime(), false));
    roster->GetMessage()->AddMessage(action);
    emit GET_CLIENT->sigMessageUpdate(call->GetId());
}
