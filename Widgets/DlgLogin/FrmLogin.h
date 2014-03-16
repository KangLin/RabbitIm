#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QFrame>
#include <QMessageBox>
#include "../../MainWindow.h"

namespace Ui {
class CFrmLogin;
}

class MainWindow;

class CFrmLogin : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmLogin(QWidget *parent = 0);
    ~CFrmLogin();

    int SetPrompt(QString szPrompt);

private slots:
    void on_pbOk_clicked();

    void on_pbClose_clicked();

private:
    Ui::CFrmLogin *ui;
};

#endif // FRMLOGIN_H
