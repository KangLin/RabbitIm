#ifndef FRMCREATEGROUPCHATROOM_H
#define FRMCREATEGROUPCHATROOM_H

#include <QDialog>
#include "FrmGroupChat.h"

namespace Ui {
class CDlgCreateGroupChatRoom;
}

class CDlgCreateGroupChatRoom : public QDialog
{
    Q_OBJECT
public:
    explicit CDlgCreateGroupChatRoom(QWidget *parent = 0);
    ~CDlgCreateGroupChatRoom();

signals:
    //void sigJoinedGroup(const QString &jid, CFrmGroupChat *pChat);

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent*);
    void changeEvent(QEvent*);

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();

    void slotError(const QString &szId, CGroupChat::Condition c, int ErrCode);
private:
    Ui::CDlgCreateGroupChatRoom *ui;
};

#endif // FRMCREATEGROUPCHATROOM_H
