#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "Widgets/DlgLogin/DlgLogin.h"
#include "Widgets/DlgLogin/FrmLogin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *);

private:
    Ui::MainWindow *ui;

    CFrmLogin* log;
};

#endif // MAINWINDOW_H
