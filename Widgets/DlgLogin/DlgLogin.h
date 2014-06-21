#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>

namespace Ui {
class CDlgLogin;
}

class CDlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgLogin(QWidget *parent = 0);
    ~CDlgLogin();

private:
    Ui::CDlgLogin *ui;
};

#endif // DLGLOGIN_H
