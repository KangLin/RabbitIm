#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>

class CGlobal : public QObject
{
    Q_OBJECT
public:
    explicit CGlobal(QObject *parent = 0);
    virtual ~CGlobal();

    QString m_szLocalJid;

signals:

public slots:

};

#endif // GLOBAL_H
