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
    explicit CFrmUservCard(QWidget *parent = 0);
    explicit CFrmUservCard(CRoster* pRoster, bool bModify = false, CXmppClient* pClient = NULL, QWidget* parent = NULL);
    ~CFrmUservCard();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
   
private slots:
    void on_pbBrower_clicked();    
    void on_pbClear_clicked();    
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    
private:
    Ui::CFrmUservCard *ui;
    CRoster* m_pRoster;
    bool m_bModify;
    CXmppClient* m_pClient;
    QBuffer m_Buffer;
};

#endif // FRMUSERVCARD_H
