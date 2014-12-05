#ifndef FRMABOUT_H
#define FRMABOUT_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class CDlgAbout;
}

class CDlgAbout : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgAbout(QWidget *parent = 0);
    ~CDlgAbout();

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent*);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void changeEvent(QEvent *e);

private slots:
    void on_btbButtons_clicked(QAbstractButton *button);

private:
    Ui::CDlgAbout *ui;
};

#endif // FRMABOUT_H
