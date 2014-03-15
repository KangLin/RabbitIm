#include "FrmLogin.h"
#include "ui_FrmLogin.h"
#include <QMessageBox>

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

void CFrmLogin::on_pbOk_clicked()
{

}

void CFrmLogin::on_pbClose_clicked()
{
    QMessageBox msg(QMessageBox::Question,
                    tr("Close"),
                    tr("Is close the programe?"),
                    QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == msg.exec())
    {
        //TODO:加入退出程序代码

    }
}
