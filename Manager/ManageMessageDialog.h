#ifndef MANAGEMESSAGEDIALOG_H
#define MANAGEMESSAGEDIALOG_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include "Widgets/FrmMessage/FrmMessage.h"

class CManageMessageDialog : public QObject
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

    virtual int CloaseDialog(const QString& szId);

private:
    QMap<QString, QSharedPointer<QFrame> > m_DlgMessage;
};

#endif // MANAGEMESSAGEDIALOG_H
