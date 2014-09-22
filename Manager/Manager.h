#ifndef MANAGER_H
#define MANAGER_H
#include "Client/Client.h"
#include "Global/GlobalUser.h"

class CManager
{
public:
    CManager();
    virtual QSharedPointer<CClient> GetClient() = 0;
    virtual QSharedPointer<CGlobalUser> GetGlobalUser() = 0;
};

#endif // MANAGER_H
