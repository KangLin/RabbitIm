#include "Client.h"
#include "Global/Global.h"

CClient::CClient(QObject *parent) :
    QObject(parent)
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
