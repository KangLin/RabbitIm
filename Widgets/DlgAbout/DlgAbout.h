#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class CDlgAbout;
}

class CDlgAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgAbout(QWidget *parent = 0);
    ~CDlgAbout();
    
private slots:
    void on_pbSave_clicked();
    
private:
    Ui::CDlgAbout *ui;
    QImage m_Image;

    int AppendFile(QTextEdit *pEdit, const QString &szFile);
};

#endif // DLGABOUT_H
