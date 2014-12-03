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
    //用于查询组成员信息  
    explicit CDlgUservCard(const QString &jid, QWidget *parent = 0);
    //用于查询好友信息  
    explicit CDlgUservCard(QSharedPointer<CUserInfo> user, bool bModify = false, QWidget* parent = NULL);
    ~CDlgUservCard();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void changeEvent(QEvent*);

private slots:
    void on_pbBrower_clicked();    
    void on_pbClear_clicked();    
    void on_pbOK_clicked();
    void on_pbCancel_clicked();

    //用于查询组成员信息,得到好友详细信息时触发  
    void slotUpdateRoster(const QString& szId, QSharedPointer<CUser> userInfo);
    int Init();

private:
    Ui::CDlgUservCard *ui;
    QString m_szJid;
    QSharedPointer<CUserInfo> m_UserInfo;
    bool m_bModify;
    QBuffer m_Buffer;
};

#endif // FRMUSERVCARD_H
