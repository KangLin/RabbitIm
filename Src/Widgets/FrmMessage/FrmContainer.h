#ifndef FRMCONTAINER_H
#define FRMCONTAINER_H

#include <QFrame>
#include <QMap>
#include <QTabWidget>
#include "UserInfo/User.h"

namespace Ui {
class CFrmContainer;
}

class CFrmContainer : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmContainer(QWidget *parent = 0);
    ~CFrmContainer();

    /**
     * @brief 查找并显示对话,如果没找到，还有空间的话，就新建一个消息框对象  
     *
     * @param szId：好友ID  
     * @return int：查找并显示对话框返回0，没找到返回-1，其它值为错误  
     */
    int ShowDialog(const QString& szId);

signals:
    /**
     * @brief 本窗口关闭,通知父容器删除自己  
     *
     * @param obj:此对象指针  
     */
    void sigClose(CFrmContainer* obj);

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void slotCloseTable(int nIndex);
    void slotCurrentChanged(int index);
    //刷新控件  
    void slotRefresh();
    void SlotChangedStatus(const QString&szId);
    void slotUpdateRoster(const QString& szId, QSharedPointer<CUser> user);
    void slotDeleteFrame(QFrame *frame);
    void slotRemove(const QString& szId);
private:
    Ui::CFrmContainer *ui;
    QMap<QString, QFrame* >m_Frame;
    QTabWidget m_tabWidget;
    int m_nSize;//容纳子窗口大小  
    bool m_bClose;//标志退出  
};

#endif // FRMCONTAINER_H
