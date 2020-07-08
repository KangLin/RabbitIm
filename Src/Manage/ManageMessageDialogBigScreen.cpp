#include "ManageMessageDialogBigScreen.h"
#include "Global/Global.h"

CManageMessageDialogBigScreen::CManageMessageDialogBigScreen(QObject *parent) : CManageMessageDialog(parent)
{
}

CManageMessageDialogBigScreen::~CManageMessageDialogBigScreen()
{
    LOG_MODEL_DEBUG("CManageMessageDialogBigScreen", "CManageMessageDialogBigScreen::~CManageMessageDialogBigScreen()");
    LogoutClean();
}

int CManageMessageDialogBigScreen::LoginInit(const QString &szId)
{
    Q_UNUSED(szId);
    return 0;
}

int CManageMessageDialogBigScreen::LogoutClean()
{
    LOG_MODEL_DEBUG("CManageMessageDialogBigScreen", "CManageMessageDialogBigScreen::Clean()");
    std::list<CFrmContainer*>::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        delete *it;
    }
    m_Container.clear();
    return 0;
}

int CManageMessageDialogBigScreen::ShowDialog(const QString &szId)
{
    int nRet = 0;
    std::list<CFrmContainer* >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        if(0 == (*it)->ShowDialog(szId))//找到  
            return 0;
    }

    //新建容器窗口对象  
    CFrmContainer* container = new CFrmContainer;
    m_Container.push_back(container);
    bool check = connect(container, SIGNAL(sigClose(CFrmContainer*)), 
                         SLOT(slotDeleteContainer(CFrmContainer*)));
    Q_ASSERT(check);

    container->ShowDialog(szId);
    return nRet;
}

void CManageMessageDialogBigScreen::slotDeleteContainer(CFrmContainer *obj)
{
    std::list<CFrmContainer* >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        if(*it == obj)
        {
            delete *it;
            m_Container.erase(it);
            return;
        }
    }
}

int CManageMessageDialogBigScreen::Show()
{
    std::list<CFrmContainer* >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        (*it)->show();
    }
    return 0;
}

int CManageMessageDialogBigScreen::Hide()
{
    std::list<CFrmContainer* >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        (*it)->hide();
    }
    return 0;
}
