/* 
 * 操作好友列表辅助类  
 * 作用：主要用于枚举操作好友列表中的成员  
 * 用法：派生此类，并在派生类中实现 ProcessRoster 成员函数  
 *      如果派生类中有多种操作方式，可以把操作方式通过para参数传递到 ProcessRoster 中。
 *      然后根据 para 参数进行操作。
 */

#ifndef COPERATEROSTER_H
#define COPERATEROSTER_H

#include "UserInfo.h"

class COperateRoster
{
public:
    explicit COperateRoster();
    virtual ~COperateRoster();
    //返回值为0,继续操作.非0,停止操作  
    virtual int ProcessRoster(QSharedPointer<CUserInfo> roster, void *para = NULL) = 0;
private:
};

#endif // COPERATEROSTER_H
