#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QFrame>
#include "../FrmUserList/FrmUserList.h"
#include "../FrmGroupChat/FrmGroupChatList.h"

namespace Ui {
class CFrmMain;
}

class CFrmMain : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmMain(QWidget *parent = 0);
    ~CFrmMain();

    CRoster* GetRoster(QString jid);
    int ShowMessageDialog();

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent*);

private:
    Ui::CFrmMain *ui;
    
    CFrmUserList m_UserList;
    CFrmGroupChatList m_GroupChatList;
};

#endif // FRMMAIN_H
