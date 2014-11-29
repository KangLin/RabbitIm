#ifndef DLGJOINGROUPCHAT_H
#define DLGJOINGROUPCHAT_H

#include <QDialog>

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
    
    void on_txtId_textEdited(const QString &arg1);
    
    void on_txtId_editingFinished();
    
private:
    Ui::CDlgJoinGroupChat *ui;
};

#endif // DLGJOINGROUPCHAT_H
