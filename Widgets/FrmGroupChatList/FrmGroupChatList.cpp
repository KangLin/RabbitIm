#include "FrmGroupChatList.h"
#include "ui_FrmGroupChatList.h"

CFrmGroupChatList::CFrmGroupChatList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatList)
{
    ui->setupUi(this);
}

CFrmGroupChatList::~CFrmGroupChatList()
{
    delete ui;
}
