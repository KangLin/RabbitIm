#include "ManageCall.h"
#include "Client/Client.h"
#include "Global/Global.h"
#include "CallAction.h"
#include <QMessageBox>
#include "MainWindow.h"
#include "UserInfo/UserInfoXmpp.h"

#undef GetMessage
CManageCall::CManageCall(QObject *parent) : CManage(parent)
{
}

CManageCall::~CManageCall()
{
    LOG_MODEL_DEBUG("Call", "CManageCall::~CManageCall");
}

int CManageCall::Init(const QString &szId)
{
    Q_UNUSED(szId);
    //TODO:初始化音视频设备  
    
    return 0;
}

int CManageCall::Clean()
{
    //TODO:清理音视频设备  
    
    return 0;
}

int CManageCall::Call(const QString &szId, bool bVideo)
{
    int nRet = 0;

    //检查是否是好友  
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageCall", "Don't get roster:%s", qPrintable(szId));
        return -1;
    }
    //检查被叫方是否在线  
    CUserInfoXmpp* pInfo = (CUserInfoXmpp*)roster->GetInfo().data();
    if(pInfo->GetResource().isEmpty() || (pInfo->GetStatus() == CUserInfo::OffLine))
    {
        LOG_MODEL_ERROR("Call", "CClientXmpp::Call the roster resource is null");
        roster->GetMessage()->AddMessage(szId, 
                tr("The roster is offline, don't launch a call."), true);
        emit GET_CLIENT->sigMessageUpdate(szId);
        return -2;
    }

    //检查呼叫是否已存在  
    if(m_Call.find(szId) != m_Call.end())
    {
        QString szShowName = szId;
        QSharedPointer<CUser> callRoster = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!callRoster.isNull())
            szShowName = callRoster->GetInfo()->GetShowName();
        roster->GetMessage()->AddMessage(szId, 
                tr("Being talk with %1, please stop it.").arg(szShowName ),
                true);
        emit GET_CLIENT->sigMessageUpdate(szId);
        return -3;
    }

    QSharedPointer<CCallObject> call;
    //具体协议实现呼叫  
    nRet = OnCall(szId, call, bVideo);
    if(nRet)
        return nRet;

    if(!call.isNull())
    {
        //增加一个呼叫消息，并增加到管理 map 中  
        m_Call.insert(szId, call);
        //增加一个呼叫通知消息  
        QSharedPointer<CCallAction> action(new CCallAction(call,
                                   szId, QTime::currentTime(), true));
        roster->GetMessage()->AddMessage(action);
        emit GET_CLIENT->sigMessageUpdate(szId);
    }
    return nRet;
}

/**
 * @brief 用于完成接收到呼叫消息的动作。  
 *        由具体的协议调用此方法。
 * @param call
 */
void CManageCall::slotCallReceived(QSharedPointer<CCallObject> call)
{
    //检查是否是好友  
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(call->GetId());
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("Call", "Don't get roster:%s", qPrintable(call->GetId()));
        return;
    }
    //检查是否正在视频  
    if(m_Call.find(call->GetId()) != m_Call.end())
    {
        /*只允许单个呼叫  
        QString szCallId = call->GetId();
        QString szShowName = szCallId;
        QSharedPointer<CUser> callRoster = GLOBAL_USER->GetUserInfoRoster(szCallId);
        if(!callRoster.isNull())
            szShowName = callRoster->GetInfo()->GetShowName();
        QString szMsg = tr("Has new call from %1. but being talk with %2, please stop it.").arg(roster->GetInfo()->GetShowName(), szShowName);
        roster->GetMessage()->AddMessage(call->GetId(), szMsg, true);
        GET_MAINWINDOW->ShowTrayIconMessage(roster->GetInfo()->GetShowName(), szMsg);
        emit GET_CLIENT->sigMessageUpdate(call->GetId());
        //*/
        LOG_MODEL_ERROR("Call", "The call [%s] is exist.", qPrintable(call->GetId()));
        return;
    }

    //新建一个呼叫对象  
    m_Call.insert(call->GetId(), call);
    //关联信号  
    bool check = connect(call.data(), SIGNAL(sigFinished(CCallObject*)),
                         SLOT(slotCallFinished(CCallObject*)));
    Q_ASSERT(check);

    //监控模式下自动接收  
    if(roster->GetInfo()->GetIsMonitor() && CGlobal::Instance()->GetIsMonitor())
    {
        call->Accept();
    }

    //添加通知消息  
    QSharedPointer<CCallAction> action(new CCallAction(call, call->GetId(), QTime::currentTime(), false));
    roster->GetMessage()->AddMessage(action);
    GET_MAINWINDOW->ShowTrayIconMessage(roster->GetInfo()->GetShowName(), 
                                        roster->GetInfo()->GetShowName() + tr(" is calling"));
    emit GET_CLIENT->sigMessageUpdate(call->GetId());
}

void CManageCall::slotCallFinished(CCallObject *pCall)
{
    LOG_MODEL_DEBUG("CManageCall", "CManageCall::slotCallFinished");
    m_Call.remove(pCall->GetId());
}

bool CManageCall::IsRun(QString szId)
{
    QMap<QString, QSharedPointer<CCallObject> >::iterator it = m_Call.find(szId);
    if(m_Call.end() != it)
    {
        return true;
    }
    return false;
}

int CManageCall::Accept(QString szId)
{
    QMap<QString, QSharedPointer<CCallObject> >::iterator it = m_Call.find(szId);
    if(m_Call.end() != it)
        return it.value()->Accept();
    return -1;
}

int CManageCall::Stop(QString szId)
{
    QMap<QString, QSharedPointer<CCallObject> >::iterator it = m_Call.find(szId);
    if(m_Call.end() != it)
        return it.value()->Stop();
    return -1;
}

/**
 * @brief 根据命令串执行操作  
 * @param szId
 * @param szCommand: accept、cancel、call
 * @return int
 * @see CFrmMessage::slotAnchorClicked
 * @see CCallAction:包含命令  
 */
int CManageCall::ProcessCommandCall(const QString &szId, const QString &szCommand)
{
    QString szCmd, szVideo;
    QStringList lstCommand = szCommand.split("&");
    szCmd = lstCommand.at(0).split("=").at(1);
    if(lstCommand.size() >= 2)
        szVideo = lstCommand.at(1).split("=").at(1);

    if("call" == szCmd)
    {
        if("true" == szVideo)
            return Call(szId, true);
        else
            return Call(szId);
    }

    QMap<QString, QSharedPointer<CCallObject> >::iterator it = m_Call.find(szId);
    if(m_Call.end() == it)
    {
        LOG_MODEL_ERROR("Call", "Hasn't the call:%s", qPrintable(szId));
        return -1;
    }

    QSharedPointer<CCallObject> call = it.value();
    if(call.isNull())
    {
        LOG_MODEL_ERROR("Call", "Hasn't the call:%s. szCmd:%s",
                        qPrintable(szId), qPrintable(szCmd));
        return -1;
    }

    if("accept" == szCmd)
        call->Accept();
    else if("cancel" == szCmd)
        call->Stop();
    else
    {
        LOG_MODEL_DEBUG("Call", "command isn't exist.szCmd:%s", qPrintable(szCmd));
        return -1;
    }
    return 0;
}
