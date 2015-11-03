#ifndef FRMUSERVCARD_H
#define FRMUSERVCARD_H

#include <QDialog>
#include <QBuffer>
#include <QSharedPointer>
#include "Client/Client.h"
#include "../../UserInfo/UserInfo.h"

namespace Ui {
class CDlgUservCard;
}

class CDlgUservCard : public QDialog
{
    Q_OBJECT

public:
    //用于编辑本地用户信息  
    explicit CDlgUservCard(QWidget *parent = 0);
    //用于查询指定ID的信息（适用于不是好友的信息查询），如果ID是好友，则会更新它的信息；如果不是好友，不会影响  
    explicit CDlgUservCard(const QString &jid, QWidget *parent = 0);
    //用于查询好友信息  
    explicit CDlgUservCard(QSharedPointer<CUserInfo> user, bool bModify = false, QWidget* parent = NULL);
    ~CDlgUservCard();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void changeEvent(QEvent*);

private slots:
    void on_pbBrowse_clicked();
    void on_pbClear_clicked();    
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    void on_pbSaveAs_clicked(); //二维码别存为  

    //用于查询组成员信息,得到好友详细信息时触发  
    void slotUpdateRoster(const QString& szId, QSharedPointer<CUser> userInfo);
    int Init();
    
private:
    Ui::CDlgUservCard *ui;
    QString m_szJid;
    QSharedPointer<CUserInfo> m_UserInfo;
    bool m_bModify;
    QBuffer m_PhotoBuffer;//头像  
    QImage m_Image; //二维码图像  
};

#endif // FRMUSERVCARD_H
