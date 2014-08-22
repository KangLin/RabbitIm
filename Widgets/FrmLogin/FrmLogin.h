#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QFrame>
#include <QMessageBox>
#include <QTimer>
#include "../../MainWindow.h"
#include "../FrmRegister/FrmRegister.h"

class MainWindow;

namespace Ui {
class CFrmLogin;
}

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
    void slotActionGroupStatusTriggered(QAction*pAct);

    void on_cmbUser_currentIndexChanged(int index);
    
    void on_pbSet_clicked();

private:
    void changeEvent(QEvent *);

private:
    Ui::CFrmLogin *ui;
    CFrmRegister *m_pRegister;
    QTimer m_tmAutoLogin;//用于自动登录

    //密码加密函数
    QString EncryptPassword(QString szPassword);
    QString DecryptPassword(QString szPassword);

    //初始化状态按钮
    int ReinitStateButton();
    QMenu m_StateMenu;
    QMap<QXmppPresence::AvailableStatusType, QAction*> m_ActionStatus;
    QActionGroup m_ActionGroupStatus;
    QXmppPresence::AvailableStatusType m_Status;
};

#endif // FRMLOGIN_H
