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

    int Init(QSet<QString> groups, QString szId = QString(), bool bRequest = false);

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();

    void on_txtJID_editingFinished();

private:
    void changeEvent(QEvent *);

private:
    Ui::CFrmAddRoster *ui;
    bool m_bRequest;
};

#endif // FRMADDROSTER_H
