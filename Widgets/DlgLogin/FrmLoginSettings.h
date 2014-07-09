#ifndef FRMLOGINSETTINGS_H
#define FRMLOGINSETTINGS_H

#include <QFrame>

namespace Ui {
class CFrmLoginSettings;
}

class CFrmLoginSettings : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmLoginSettings(QWidget *parent = 0);
    ~CFrmLoginSettings();

    int SetLogin(QWidget *pLogin);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pbOK_clicked();

    void on_pbCanecl_clicked();

private:
    Ui::CFrmLoginSettings *ui;

    QWidget* m_pParent;
};

#endif // FRMLOGINSETTINGS_H
