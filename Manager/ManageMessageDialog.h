#ifndef MANAGEMESSAGEDIALOG_H
#define MANAGEMESSAGEDIALOG_H

#include <QMap>
#include <QSharedPointer>
#include "Widgets/FrmMessage/FrmMessage.h"

class CManageMessageDialog
{
public:
    CManageMessageDialog();
    virtual ~CManageMessageDialog();

    /**
     * @brief 用户登录成功后调用初始化  
     *
     * @param szId：登录用户  
     * @return int：成功返回0，失败返回非0  
     */
    int Init(const QString &szId);
    /**
     * @brief 用户登出时做清理工作  
     *
     * @return int：成功返回0，失败返回非0  
     */
    int Clean();
    /**
     * @brief 显示指定好友对话框  
     *
     * @param szId：好友ID  
     * @return int：成功返回0，失败返回非0  
     */
    int ShowDialog(const QString& szId);

private:
    QMap<QString, QSharedPointer<CFrmMessage> > m_DlgMessage;
};

#endif // MANAGEMESSAGEDIALOG_H
