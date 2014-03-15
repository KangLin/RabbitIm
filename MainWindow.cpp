#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pLogin = new CFrmLogin(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    m_pLogin->setFixedSize(this->geometry().width(),
                           this->geometry().height());
}

void MainWindow::showEvent(QShowEvent *)
{
}
