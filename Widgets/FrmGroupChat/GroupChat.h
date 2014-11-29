#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QObject>

class CGroupChat : public QObject
{
    Q_OBJECT
public:
    explicit CGroupChat(QObject *parent = 0);
    virtual QString Id() = 0;
    virtual QString ShowName() = 0;
    virtual QString Nick() = 0;
    virtual int Leave() = 0;

signals:
    
public slots:
    
};

#endif // GROUPCHAT_H
