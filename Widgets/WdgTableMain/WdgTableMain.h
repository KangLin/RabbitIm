#ifndef WDGTABLEMAIN_H
#define WDGTABLEMAIN_H

#include <QTabWidget>
#include "../FrmUserList/FrmUserList.h"
#include "../FrmGroupChat/FrmGroupChatList.h"

namespace Ui {
class CWdgTableMain;
}

class CWdgTableMain : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit CWdgTableMain(QWidget *parent = 0);
    ~CWdgTableMain();

    CRoster* GetRoster(QString jid);

private:
    Ui::CWdgTableMain *ui;
    CFrmUserList m_UserList;
    CFrmGroupChatList m_GroupChatList;
};

#endif // WDGTABLEMAIN_H
