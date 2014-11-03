#ifndef FRMREGISTER_H
#define FRMREGISTER_H

#include <QDialog>
#include "Client/Client.h"

namespace Ui {
class CDlgRegister;
}

class CDlgRegister : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgRegister(QWidget *parent = 0);
    ~CDlgRegister();

    int SetLogin(QWidget *pLogin);

protected:
    virtual void hideEvent(QHideEvent *);
    virtual void changeEvent(QEvent*);

private slots:
    void connected();
    void clientError(CClient::ERROR_TYPE e);
    void on_pbCreate_clicked();
    void on_pbCancel_clicked();

private:
    Ui::CDlgRegister *ui;
    QWidget *m_pLogin;
};

#endif // FRMREGISTER_H
