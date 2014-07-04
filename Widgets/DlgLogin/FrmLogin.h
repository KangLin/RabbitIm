#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QFrame>
#include <QMessageBox>
#include <QTimer>
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
    void slotOnLineTriggered();
    void slotChatTriggered();
    void slotTemporarilyawayTriggered();
    void slotDoNotDisturbTriggered();
    void slotInvisibleTriggered();

private:
    Ui::CFrmLogin *ui;
    CFrmRegister *m_pRegister;
    QTimer m_tmAutoLogin;//用于自动登录

    //密码加密函数
    QString EncryptPassword(QString szPassword);
    QString DecryptPassword(QString szPassword);

    //初始化状态按钮
    int InitStateButton();
    QMenu m_StateMenu;
    QXmppPresence::AvailableStatusType m_Status;
};

#endif // FRMLOGIN_H
