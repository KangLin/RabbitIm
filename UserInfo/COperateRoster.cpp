
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
    LOG_MODEL_DEBUG("operateRoster", "must overwrite the function");
    return 0;
}
