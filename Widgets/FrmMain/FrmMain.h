#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QFrame>
#include "../FrmUserList/FrmUserList.h"
#include "../FrmGroupChat/FrmGroupChatList.h"

namespace Ui {
class CFrmMain;
}

class MainWindow;
class CFrmMain : public QFrame
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit CFrmMain(QWidget *parent = 0);
    ~CFrmMain();

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent*);

    int Init();

private:
    Ui::CFrmMain *ui;
    
    CFrmUserList m_UserList;
    //CFrmGroupChatList m_GroupChatList;
};

#endif // FRMMAIN_H
