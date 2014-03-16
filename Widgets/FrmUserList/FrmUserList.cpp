#include "FrmUserList.h"
#include "ui_FrmUserList.h"

CFrmUserList::CFrmUserList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);
}

CFrmUserList::~CFrmUserList()
{
    delete ui;
}
