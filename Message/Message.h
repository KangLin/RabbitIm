#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class CMessage : public QObject
{
    Q_OBJECT
public:
    explicit CMessage(QObject *parent = 0);

    int GetUnReadCount();
    
signals:
    
public slots:
    
};

#endif // MESSAGE_H
