#ifndef FRMLOGINSETTINGS_H
#define FRMLOGINSETTINGS_H

#include <QDialog>

namespace Ui {
class CDlgLoginSettings;
}

/*本类在窗口关闭时会自己释放内存，所以构造时请用
 * CFrmLoginSettings* p = new CFrmLoginSettings;
 * p->SetLogin(this);
 * p->show();
 */
class CDlgLoginSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgLoginSettings(QWidget *parent = 0);
    ~CDlgLoginSettings();

    int SetLogin(QWidget *pLogin);

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent (QShowEvent* event);
    virtual void changeEvent(QEvent*);

private slots:
    void on_pbOK_clicked();

    void on_pbCanecl_clicked();

private:
    Ui::CDlgLoginSettings *ui;
};

#endif // FRMLOGINSETTINGS_H
