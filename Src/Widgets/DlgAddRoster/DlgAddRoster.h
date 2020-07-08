#ifndef FRMADDROSTER_H
#define FRMADDROSTER_H

#include <QDialog>

namespace Ui {
class CDlgAddRoster;
}

class CDlgAddRoster : public QDialog
{
    Q_OBJECT

public:
    explicit CDlgAddRoster(QWidget *parent = 0);
    ~CDlgAddRoster();

    //初始化  
    //参数:  
    //        groups:组  
    //        szId:好友的ID  
    //        bRequest:是否是请求  
    int Init(QSet<QString> groups, QString szId = QString(), bool bRequest = false);

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();

    void on_txtJID_editingFinished();

private:
    void changeEvent(QEvent *);

private:
    Ui::CDlgAddRoster *ui;
    bool m_bRequest;
};

#endif // FRMADDROSTER_H
