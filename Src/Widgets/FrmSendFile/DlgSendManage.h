#ifndef DLGSENDMANAGE_H
#define DLGSENDMANAGE_H

#include <QDialog>
#include "qxmpp/QXmppTransferManager.h"

namespace Ui {
class CDlgSendManage;
}

class CDlgSendManage : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgSendManage(QWidget *parent = 0);
    ~CDlgSendManage();
    void addFileProcess(QXmppTransferJob &job, bool isSend);

private slots:
    void onSigClosed();

private:
    Ui::CDlgSendManage *ui;
    int m_SendCount;
};

#endif // DLGSENDMANAGE_H

