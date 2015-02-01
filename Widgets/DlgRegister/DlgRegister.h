#ifndef FRMREGISTER_H
#define FRMREGISTER_H

#include <QDialog>
#include <QBuffer>
#include "Client/Client.h"

namespace Ui {
class CDlgRegister;
}

class CDlgRegister : public QDialog
{
    Q_OBJECT

public:
    //parent:必须为 CFrmLogin 对象指针  
    explicit CDlgRegister(QWidget *parent = 0);
    ~CDlgRegister();

protected:
    virtual void hideEvent(QHideEvent *);
    virtual void changeEvent(QEvent*);

private slots:
    void connected();
    void clientError(CClient::ERROR_TYPE e);
    void on_pbCreate_clicked();
    void on_pbCancel_clicked();

    void on_pbBrower_clicked();
    
    void on_pbClear_clicked();
    
private:
    Ui::CDlgRegister *ui;
    QWidget *m_pLogin;
    QBuffer m_PhotoBuffer;//头像  
};

#endif // FRMREGISTER_H
