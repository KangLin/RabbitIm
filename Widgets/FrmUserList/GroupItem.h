#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QStandardItem>

class CGroupItem : public QStandardItem
{
public:
    CGroupItem(QString &text, QString &key);
    virtual ~CGroupItem();

private:
    QString m_szKey;
};

#endif // GROUPITEM_H
