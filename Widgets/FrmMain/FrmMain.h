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

    //TODO:以后放在未读消息中维护  
    int ShowMessageDialog();

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent*);

    int Init();

    //查找好友对象  
    CRoster* GetRoster(QString jid);
    //查找聊天室对象  
    CFrmGroupChat* GetGroupChat(const QString& jid);
private:
    Ui::CFrmMain *ui;
    
    CFrmUserList m_UserList;
    CFrmGroupChatList m_GroupChatList;
};

#endif // FRMMAIN_H
