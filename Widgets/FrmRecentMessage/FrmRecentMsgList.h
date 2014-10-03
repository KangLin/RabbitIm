#ifndef FrmRecentMsgList_H
#define FrmRecentMsgList_H

#include <QFrame>
#include <QString>
#include <QStandardItemModel>
#include <QMap>
#include <QList>
#include <QResizeEvent>
#include <QMutex>
#include "CustomTreeView.h"
#include "UserInfo/User.h"

namespace Ui {
class CFrmRecentMsgList;
}

class CFrmRecentMsgList : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmRecentMsgList(QWidget *parent = 0);
    ~CFrmRecentMsgList();

private slots:
    void slotMessageUpdate(const QString& szId);
    void LoadFromStorage();
    //好友出席状态改变时触发  
    void SlotChangedStatus(const QString& szId);

private slots:
    //树形列表控件响应事件 
    void clicked(const QModelIndex & index);
    void doubleClicked(const QModelIndex & index);

private:
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent* e);

    enum _OPERATOR_TYPE
    {
        OPERATE_TYPE_INSERT_ROSTER,
        OPERATE_TYPE_UPDATE_ROSTER
    };

    enum _USERLIST_ROLE
    {
        USERLIST_ITEM_ROLE_JID = Qt::UserRole + 1,
        USERLIST_ITEM_ROLE_PROPERTIES = USERLIST_ITEM_ROLE_JID + 1
    };

    enum _PROPERTIES
    {
        PROPERTIES_GROUP,
        PROPERTIES_ROSTER,
        PROPERTIES_UNREAD_MESSAGE_COUNT
    };

    int ItemInsertRoster(QSharedPointer<CUser> roster, int nRow = 0);

private:
    Ui::CFrmRecentMsgList *ui;

    CCustomTreeView m_MsgList;
    QStandardItemModel *m_pModel;       //列表树型控件  
};

#endif // FrmRecentMsgList_H
