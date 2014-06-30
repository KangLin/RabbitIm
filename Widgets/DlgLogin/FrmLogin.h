#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QFrame>
#include <QMessageBox>
#include "../../MainWindow.h"
#include "../FrmRegister/FrmRegister.h"

namespace Ui {
class CFrmLogin;
}

class MainWindow;

class CFrmLogin : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmLogin(QWidget *parent = 0);
    ~CFrmLogin();

    int SetPrompt(QString szPrompt);
    int SetLoginInformation(QString szName, QString szPassword);
    int SaveConf();

private slots:
    void on_pbOk_clicked();
    void on_pbClose_clicked();
    void on_pbRegitster_clicked();
    
    void on_chkLogin_stateChanged(int state);
    
private:
    Ui::CFrmLogin *ui;
    CFrmRegister *m_pRegister;
    
    QString EncryptPassword(QString szPassword);
    QString DecryptPassword(QString szPassword);
};

#endif // FRMLOGIN_H
