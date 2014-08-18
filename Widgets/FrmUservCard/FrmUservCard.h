#ifndef FRMUSERVCARD_H
#define FRMUSERVCARD_H

#include <QFrame>
#include <QBuffer>
#include "../FrmUserList/Roster.h"
#include "../../XmppClient.h"

namespace Ui {
class CFrmUservCard;
}

class CFrmUservCard : public QFrame
{
    Q_OBJECT

public:
    //用于编辑本地用户信息  
    explicit CFrmUservCard(QWidget *parent = 0);
    //用于查询组成员信息  
    explicit CFrmUservCard(const QString &jid, QWidget *parent = 0);
    //用于查询好友信息  
    explicit CFrmUservCard(CRoster* pRoster, bool bModify = false, QWidget* parent = NULL);
    ~CFrmUservCard();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void changeEvent(QEvent*);

private slots:
    void on_pbBrower_clicked();    
    void on_pbClear_clicked();    
    void on_pbOK_clicked();
    void on_pbCancel_clicked();

    //得到好友详细信息时触发  
    void slotvCardReceived(const QXmppVCardIq&);

private:
    Ui::CFrmUservCard *ui;
    QString m_szJid;
    CRoster* m_pRoster;
    bool m_bModify;
    CXmppClient* m_pClient;
    QBuffer m_Buffer;
};

#endif // FRMUSERVCARD_H
