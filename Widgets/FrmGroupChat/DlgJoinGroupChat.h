#ifndef DLGJOINGROUPCHAT_H
#define DLGJOINGROUPCHAT_H

#include <QDialog>
#include "GroupChat.h"

namespace Ui {
class CDlgJoinGroupChat;
}

class CDlgJoinGroupChat : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgJoinGroupChat(QWidget *parent = 0);
    ~CDlgJoinGroupChat();
    
private slots:
    void on_buttonBox_accepted();
    void slotError(const QString &szId, CGroupChat::Condition c);

private:
    Ui::CDlgJoinGroupChat *ui;
};

#endif // DLGJOINGROUPCHAT_H
