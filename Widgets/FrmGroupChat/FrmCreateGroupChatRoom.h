#ifndef FRMCREATEGROUPCHATROOM_H
#define FRMCREATEGROUPCHATROOM_H

#include <QFrame>
#include "FrmGroupChat.h"

namespace Ui {
class CFrmCreateGroupChatRoom;
}

class CFrmCreateGroupChatRoom : public QFrame
{
    Q_OBJECT
public:
    static CFrmCreateGroupChatRoom* Instance(const QString& jid);
    int Init(const QString &jid);

private:
    explicit CFrmCreateGroupChatRoom(QWidget *parent = 0);
    ~CFrmCreateGroupChatRoom();

signals:
    void sigJoinedGroup(const QString &jid, CFrmGroupChat *pChat);

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent*);

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();

private:
    Ui::CFrmCreateGroupChatRoom *ui;
    QString m_szJid;
};

#endif // FRMCREATEGROUPCHATROOM_H
