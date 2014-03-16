#ifndef FRMUSERLIST_H
#define FRMUSERLIST_H

#include <QFrame>

namespace Ui {
class CFrmUserList;
}

class CFrmUserList : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmUserList(QWidget *parent = 0);
    ~CFrmUserList();

private:
    Ui::CFrmUserList *ui;
};

#endif // FRMUSERLIST_H
