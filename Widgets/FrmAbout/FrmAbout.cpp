#include "FrmAbout.h"
#include "ui_FrmAbout.h"

CFrmAbout::CFrmAbout(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmAbout)
{
    ui->setupUi(this);
}

CFrmAbout::~CFrmAbout()
{
    qDebug("CFrmAbout::~CFrmAbout");
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
