/* 
 * 操作好友列表辅助类  
 * 作用：主要用于枚举操作好友列表中的成员  
 * 用法：派生此类，并在派生类中实现 Proecess 成员函数  
 * 
 */

#ifndef COPERATEROSTER_H
#define COPERATEROSTER_H

#include "UserInfoRoster.h"

class COperateRoster
{
public:
    explicit COperateRoster();
    virtual ~COperateRoster();
    //返回值为0,继续操作.非0,停止操作  
    virtual int ProcessRoster(QSharedPointer<CUserInfoRoster> roster, void *para = NULL);
private:
};

#endif // COPERATEROSTER_H
