#ifndef MANAGER_H
#define MANAGER_H
#include "Client/Client.h"
#include "Global/GlobalUser.h"
#include "ManageMessageDialog.h"
#include <QSharedPointer>

class CManager
{
public:
    CManager();
    virtual ~CManager();
    virtual QSharedPointer<CClient> GetClient() = 0;
    virtual QSharedPointer<CGlobalUser> GetGlobalUser() = 0;
    virtual QSharedPointer<CManageMessageDialog> GetManageMessageDialog();

private:
    QSharedPointer<CManageMessageDialog> m_ManageMessageDialog;
};

#endif // MANAGER_H
