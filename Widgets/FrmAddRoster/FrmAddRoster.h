#ifndef FRMADDROSTER_H
#define FRMADDROSTER_H

#include <QFrame>

namespace Ui {
class CFrmAddRoster;
}

class CXmppClient;
class CFrmAddRoster : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmAddRoster(QWidget *parent = 0);
    ~CFrmAddRoster();

    int Init(CXmppClient* pClient, QSet<QString> groups, QString bareJid = QString());

private slots:
    void on_pbOk_clicked();
    void on_pushButton_clicked();

    void on_txtJID_editingFinished();

private:
    Ui::CFrmAddRoster *ui;
    CXmppClient *m_pClient;
    bool m_bRequest;
};

#endif // FRMADDROSTER_H
