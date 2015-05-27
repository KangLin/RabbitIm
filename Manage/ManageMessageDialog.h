#ifndef MANAGEMESSAGEDIALOG_H
#define MANAGEMESSAGEDIALOG_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include "Widgets/FrmMessage/FrmMessage.h"
#include "Manage.h"

/**
  * @defgroup RABBITIM_INTERFACE_MANAGEMESSAGEDIALOG 消息对活框管理类模块  
  * @ingroup RABBITIM_INTERFACE_MANAGER
  * @brief 消息对活框管理类模块，用于管理消息对话框。现在有PC屏幕和手机屏幕管理  
  */

/**
 * @class CManageMessageDialog
 * @ingroup RABBITIM_INTERFACE_MANAGEMESSAGEDIALOG RABBITIM_INTERFACE
 * @brief 消息对活框管理类(手机屏幕管理)，用于管理消息对话框。  
 * @see CManager
 */
class CManageMessageDialog : public CManage
{
    Q_OBJECT
public:
    CManageMessageDialog(QObject *parent = 0);
    virtual ~CManageMessageDialog();

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

    virtual int Hide();
    virtual int Show();

private slots:
    void slotDeleteFrmMessage(QFrame *obj);
    void slotRemove(const QString& szId);
private:
    QMap<QString, QFrame*> m_DlgMessage;
};

#endif // MANAGEMESSAGEDIALOG_H
