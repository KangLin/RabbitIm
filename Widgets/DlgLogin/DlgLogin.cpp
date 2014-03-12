#include "DlgLogin.h"
#include "ui_DlgLogin.h"

CDlgLogin::CDlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgLogin)
{
    ui->setupUi(this);
}

CDlgLogin::~CDlgLogin()
{
    delete ui;
}
