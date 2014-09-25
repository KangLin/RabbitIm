#include "Manager.h"

CManager::CManager() :
    m_ManageMessageDialog(new CManageMessageDialog)
{
}

CManager::~CManager()
{
}

QSharedPointer<CManageMessageDialog> CManager::GetManageMessageDialog()
{
    return m_ManageMessageDialog;
}
