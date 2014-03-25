#ifndef FRMREGISTER_H
#define FRMREGISTER_H

#include <QFrame>
#include "../../XmppClient.h"

namespace Ui {
class CFrmRegister;
}

class CFrmRegister : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmRegister(QWidget *parent = 0);
    ~CFrmRegister();

    int SetLogin(QWidget *pLogin);

protected:
    void hideEvent(QHideEvent *);

private slots:
    void connected();
    void clientError(QXmppClient::Error e);
    void clientIqReceived(const QXmppIq &iq);
    void on_pbCreate_clicked();
    void on_pbCancel_clicked();

private:
    Ui::CFrmRegister *ui;
    QWidget *m_pLogin;
    CXmppClient* m_pClient;
};

#endif // FRMREGISTER_H
