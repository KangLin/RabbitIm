#ifndef FRMGROUPCHATLIST_H
#define FRMGROUPCHATLIST_H

#include <QFrame>

namespace Ui {
class CFrmGroupChatList;
}

class CFrmGroupChatList : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmGroupChatList(QWidget *parent = 0);
    ~CFrmGroupChatList();
    
private:
    Ui::CFrmGroupChatList *ui;
};

#endif // FRMGROUPCHATLIST_H
