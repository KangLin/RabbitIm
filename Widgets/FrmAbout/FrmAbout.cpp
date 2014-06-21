#include "FrmAbout.h"
#include "ui_FrmAbout.h"
#include "../../Global.h"

CFrmAbout::CFrmAbout(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmAbout)
{
    ui->setupUi(this);
    
    //设置背景
    g_Global.SetStyleSheet(this);
}

CFrmAbout::~CFrmAbout()
{
    LOG_MODEL_DEBUG("About", "CFrmAbout::~CFrmAbout");
    delete ui;
}

void CFrmAbout::closeEvent(QCloseEvent *)
{
    deleteLater();
}

void CFrmAbout::on_btbButtons_clicked(QAbstractButton *button)
{
    Q_UNUSED(button);
    deleteLater();
}
