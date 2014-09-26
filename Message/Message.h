#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class CMessage : public QObject
{
    Q_OBJECT
public:
    explicit CMessage(QObject *parent = 0);

    ///< 得到未读消息数  
    int GetUnReadCount();
    ///< 增加消息  
    ///< 读取消息  
    ///< 读取所有未读消息  

signals:
    
public slots:
    
};

#endif // MESSAGE_H
