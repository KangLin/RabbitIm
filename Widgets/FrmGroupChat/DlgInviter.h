#ifndef DLGINVITER_H
#define DLGINVITER_H

#include <QDialog>
#include "Widgets/FrmCustom/ItemModelCustom.h"
#include <QMap>
#include "UserInfo/COperateRoster.h"
#include "GroupChat.h"

namespace Ui {
class CDlgInviter;
}

class CDlgInviter : public QDialog, COperateRoster
{
    Q_OBJECT
public:
    explicit CDlgInviter(const QString &szId, QWidget *parent = 0);
    ~CDlgInviter();
    /**
     * @brief 处理好友，COperateRoster::ProcessRoster 重载函数  
     *
     * @param roster：要处理的好友  
     * @param para：详见 enum _OPERATOR_TYPE  
     * @return int：成功返回0，失败返回非0，枚举将停止。  
     */
    virtual int ProcessRoster(QSharedPointer<CUserInfo> roster, void *para = NULL);

private slots:
    
    void on_buttonBox_accepted();
    
private:
    //传给 ProcessRoster 中的参数类型  
    enum _OPERATOR_TYPE
    {
        OPERATE_TYPE_INSERT_ROSTER,
        OPERATE_TYPE_UPDATE_ROSTER
    };

    enum _USERLIST_ROLE
    {
        USERLIST_ITEM_ROLE_ID = Qt::UserRole + 1,
        USERLIST_ITEM_ROLE_PROPERTIES = USERLIST_ITEM_ROLE_ID + 1
    };

    enum _PROPERTIES
    {
        PROPERTIES_GROUP,
        PROPERTIES_ROSTER,
        PROPERTIES_UNREAD_MESSAGE_COUNT
    };
private:
    //在组队列中插入组  
    QStandardItem* ItemInsertGroup(QString szGroup);
    int ItemInsertRoster(const QString& szId);
    int ItemUpdateGroup(QStandardItem *pItems, QSet<QString> groups);

private:
    Ui::CDlgInviter *ui;

    CItemModelCustom *m_pTreeModel;         //好友列表树型控件     
    QMap<QString, QStandardItem*> m_Groups; //组列表:<组名,QStandardItem>  
    QSharedPointer<CGroupChat> m_GroupChat;
};

#endif // DLGINVITER_H
