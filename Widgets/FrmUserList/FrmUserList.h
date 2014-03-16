#ifndef FRMUSERLIST_H
#define FRMUSERLIST_H

#include <QFrame>
#include <QStandardItemModel>

namespace Ui {
class CFrmUserList;
}

class CFrmUserList : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmUserList(QWidget *parent = 0);
    ~CFrmUserList();

    int InsertUser(QString szJid, QString szUser, QSet<QString> szGroups);
    int RemoveUser(QString szJid);

private:
    Ui::CFrmUserList *ui;

    QStandardItemModel *m_pModel;
};

#endif // FRMUSERLIST_H
