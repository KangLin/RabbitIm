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
    //主动加入  
    explicit CDlgJoinGroupChat(QWidget *parent = 0);
    //接受邀请加入  
    explicit CDlgJoinGroupChat(const QString &szGroupChatId, const QString &szInviter, const QString &szReason, QWidget *parent = 0);
    ~CDlgJoinGroupChat();
    
private slots:
    void on_buttonBox_accepted();
    void slotError(const QString &szId, CGroupChat::Condition c, int errorno);

private:
    int Init();
    
private:
    Ui::CDlgJoinGroupChat *ui;
};

#endif // DLGJOINGROUPCHAT_H
