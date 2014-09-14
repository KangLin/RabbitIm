#ifndef FRMADDROSTER_H
#define FRMADDROSTER_H

#include <QFrame>

namespace Ui {
class CFrmAddRoster;
}

class CClientXmpp;
class CFrmAddRoster : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmAddRoster(QWidget *parent = 0);
    ~CFrmAddRoster();

    int Init(CClientXmpp* pClient, QSet<QString> groups, QString bareJid = QString());

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();

    void on_txtJID_editingFinished();

private:
    void changeEvent(QEvent *);

private:
    Ui::CFrmAddRoster *ui;
    CClientXmpp *m_pClient;
    bool m_bRequest;
};

#endif // FRMADDROSTER_H
