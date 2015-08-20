#include "FrmLbsMain.h"
#include "ui_FrmLbsMain.h"

CFrmLbsMain::CFrmLbsMain(QWidget *parent) :
    QFrame(parent),
    m_Motion(this),
    m_Track(this),
    ui(new Ui::CFrmLbsMain)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    
    ui->tabWidget->addTab(&m_Motion, QIcon(":/png/motion"), tr("Motion"));
    ui->tabWidget->addTab(&m_Track, QIcon(), tr("Track"));
}

CFrmLbsMain::~CFrmLbsMain()
{
    delete ui;
}
