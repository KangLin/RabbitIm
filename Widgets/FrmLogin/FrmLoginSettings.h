#ifndef FRMLOGINSETTINGS_H
#define FRMLOGINSETTINGS_H

#include <QFrame>

namespace Ui {
class CFrmLoginSettings;
}

/*本类在窗口关闭时会自己释放内存，所以构造时请用
 * CFrmLoginSettings* p = new CFrmLoginSettings;
 * p->SetLogin(this);
 * p->show();
 */
class CFrmLoginSettings : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmLoginSettings(QWidget *parent = 0);
    ~CFrmLoginSettings();

    int SetLogin(QWidget *pLogin);

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent (QShowEvent* event);

private slots:
    void on_pbOK_clicked();

    void on_pbCanecl_clicked();

private:
    Ui::CFrmLoginSettings *ui;

    QWidget* m_pParent;
};

#endif // FRMLOGINSETTINGS_H
