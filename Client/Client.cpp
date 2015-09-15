#include "Client.h"
#include "Global/Global.h"

CClient::CClient(QObject *parent) :
    QObject(parent), COperateRoster()
{
    m_bIsLogin = false;
}

CClient::~CClient()
{
}

bool CClient::IsLogin()
{
    return m_bIsLogin;
}

int CClient::SetLogin(bool bLogin)
{
    m_bIsLogin = bLogin;
    return 0;
}

/**
 * @brief 客户端连接后进行初始化工作.注意:先初始化数据,再初始化界面  
 *
 */
void CClient::slotClientConnected(const QString &szId)
{
    int nRet = 0;
    if(!IsLogin())
    {
        //数据层在界面之前初始化  
        //在这一步中,如果以前保存过本地用户信息到存储,会在加载时初始化USER_INFO_LOCALE  
        nRet = CGlobal::Instance()->GetManager()->LoginInit(szId);
        if(nRet)
        {
            LOG_MODEL_ERROR("CClientXmpp", "Init GlobalUser fail");
            return;
        }

        //因为openfire当用户信息改变时，不会广播改变通知，所以当程序启动时要查询所有信息。这里会影响性能  
        //TODO:一种解决方案：只在查看用户信息时，才发送更新请求  
        //这是程序第一次启动,没有保存过用户信息到存储  
        if(USER_INFO_LOCALE.isNull())
        {
                //TODO:生成本地用户对象  
                //调用客户端操作，得到本地用户信息  
                GET_CLIENT->RequestUserInfoLocale();
        }

        SetLogin(true);//设置登录标志  
        emit sigClientConnected();//通知界面初始化  
        emit sigLoadRosterFromStorage();//通知界面初始化数据  
    }

    return;
}

/**
 * @brief 客户端断开连接时清理操作.注意:先清理界面,再清理数据  
 *
 */
void CClient::slotClientDisconnected()
{
    //Logout()中先设置了false,才触发此信号的  
    if(!IsLogin())
    {
        //注意:这个顺序不能变,先清理界面，再清理数据    
        emit sigClientDisconnected();
        CGlobal::Instance()->GetManager()->LogoutClean();
    }
    return;
}

int CClient::RefreshRosterList()
{
    return CGlobal::Instance()->GetManager()->GetManageUser()->ProcessRoster(this);
}

int CClient::ProcessRoster(QSharedPointer<CUserInfo> roster, void *para)
{
    if(roster.isNull())
        return -1;
    return RequestUserInfoRoster(roster->GetId());
}
