#ifndef MANAGEMESSAGEDIALOGBIGSCREEN_H
#define MANAGEMESSAGEDIALOGBIGSCREEN_H

#include <QObject>
#include "ManageMessageDialog.h"
#include <list>
#include "Widgets/FrmMessage/FrmContainer.h"

class CManageMessageDialogBigScreen : public CManageMessageDialog
{
    Q_OBJECT
public:
    CManageMessageDialogBigScreen(QObject *parent = 0);
    virtual ~CManageMessageDialogBigScreen();
    
    /**
     * @brief 用户登录成功后调用初始化  
     *
     * @param szId：登录用户  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int Init(const QString &szId);
    /**
     * @brief 用户登出时做清理工作  
     *
     * @return int：成功返回0，失败返回非0  
     */
    virtual int Clean();
    /**
     * @brief 显示指定好友对话框  
     *
     * @param szId：好友ID  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int ShowDialog(const QString& szId);

    /**
     * @brief 关闭好友对话框  
     *
     * @param szId:好友ID  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int CloaseDialog(const QString& szId);

private:
    std::list<QSharedPointer<CFrmContainer> > m_Container;
};

#endif // MANAGEMESSAGEDIALOGBIGSCREEN_H
