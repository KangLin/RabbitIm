#ifndef DLGGROUPCHATINFO_H
#define DLGGROUPCHATINFO_H

#include <QDialog>

namespace Ui {
class CDlgGroupChatInfo;
}

class CDlgGroupChatInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgGroupChatInfo(const QString &szId, QWidget *parent = 0);
    ~CDlgGroupChatInfo();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::CDlgGroupChatInfo *ui;
};

#endif // DLGGROUPCHATINFO_H
