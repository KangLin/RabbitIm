#include "FrmGroupChatFind.h"
#include "ui_FrmGroupChatFind.h"

CFrmGroupChatFind::CFrmGroupChatFind(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatFind)
{
    ui->setupUi(this);
}

CFrmGroupChatFind::~CFrmGroupChatFind()
{
    delete ui;
}
