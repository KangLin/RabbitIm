#include "FrmLogin.h"
#include "ui_FrmLogin.h"

CFrmLogin::CFrmLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmLogin)
{
    ui->setupUi(this);
}

CFrmLogin::~CFrmLogin()
{
    delete ui;
}
