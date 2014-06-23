#ifndef FRMABOUT_H
#define FRMABOUT_H

#include <QFrame>
#include <QAbstractButton>

namespace Ui {
class CFrmAbout;
}

class CFrmAbout : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmAbout(QWidget *parent = 0);
    ~CFrmAbout();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_btbButtons_clicked(QAbstractButton *button);

private:
    Ui::CFrmAbout *ui;
};

#endif // FRMABOUT_H
