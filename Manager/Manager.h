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

    /**
     * @brief 登录后初始化  
     *
     * @param szId
     * @return int
     */
    virtual int Init(const QString& szId);
    virtual int Clean();
};

#endif // MANAGER_H
