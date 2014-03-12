#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(parent)
        this->setFixedSize(parent->childrenRect().width(),
                       parent->childrenRect().height());
    log = new CFrmLogin(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    log->setFixedSize(this->childrenRect().width(),
                      this->childrenRect().height());
}

void MainWindow::showEvent(QShowEvent *)
{
    log->setFixedSize(this->childrenRect().width(),
                      this->childrenRect().height());
}
