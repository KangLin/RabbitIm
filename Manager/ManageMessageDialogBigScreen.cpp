#include "ManageMessageDialogBigScreen.h"
#include "Global/Global.h"

CManageMessageDialogBigScreen::CManageMessageDialogBigScreen(QObject *parent) : CManageMessageDialog(parent)
{
}

CManageMessageDialogBigScreen::~CManageMessageDialogBigScreen()
{
    LOG_MODEL_DEBUG("CManageMessageDialogBigScreen", "CManageMessageDialogBigScreen::~CManageMessageDialogBigScreen()");
    Clean();
}

int CManageMessageDialogBigScreen::Init(const QString &szId)
{
    Q_UNUSED(szId);
    return 0;
}

int CManageMessageDialogBigScreen::Clean()
{
    LOG_MODEL_DEBUG("CManageMessageDialogBigScreen", "CManageMessageDialogBigScreen::Clean()");
    m_Container.clear();
    return 0;
}

int CManageMessageDialogBigScreen::ShowDialog(const QString &szId)
{
    int nRet = 0;
    std::list<QSharedPointer<CFrmContainer> >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        QSharedPointer<CFrmContainer> container = *it;
        if(0 == container->ShowDialog(szId))//找到  
            return 0;
    }

    QSharedPointer<CFrmContainer> container(new CFrmContainer);
    m_Container.push_back(container);

    container->ShowDialog(szId);
    return nRet;
}

int CManageMessageDialogBigScreen::CloaseDialog(const QString &szId)
{
    LOG_MODEL_DEBUG("CManageMessageDialogBigScreen", "CManageMessageDialogBigScreen::CloaseDialog:%s", qPrintable(szId));
    int nRet = 0;
    std::list<QSharedPointer<CFrmContainer> >::iterator it;
    for(it = m_Container.begin(); it != m_Container.end(); it++)
    {
        QSharedPointer<CFrmContainer> container = *it;
        if(0 == container->CloaseDialog(szId))
         {
            if(container->GetCount() == 0)
                m_Container.erase(it);
            return 0;
        }
    }
    return nRet;
}
