#ifndef FRMGROUPCHATFIND_H
#define FRMGROUPCHATFIND_H

#include <QFrame>

namespace Ui {
class CFrmGroupChatFind;
}

class CFrmGroupChatFind : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmGroupChatFind(QWidget *parent = 0);
    ~CFrmGroupChatFind();
    
private:
    Ui::CFrmGroupChatFind *ui;
};

#endif // FRMGROUPCHATFIND_H
