
#include "COperateRoster.h"
#include "../Global/Global.h"

COperateRoster::COperateRoster()
{
}

COperateRoster::~COperateRoster()
{
}

int COperateRoster::ProcessRoster(QSharedPointer<CUserInfo> roster, void* para)
{
    Q_UNUSED(roster);
    Q_UNUSED(para);
    qWarning() << "must overwrite the function";
    return 0;
}
