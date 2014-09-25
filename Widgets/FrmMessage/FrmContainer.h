#ifndef FRMCONTAINER_H
#define FRMCONTAINER_H

#include <QFrame>
#include <QMap>
#include <QTabWidget>

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
    /**
     * @brief 关闭指定的消息框  
     *
     * @param szId：好友ID  
     * @return int：关闭成功返回0，不在这个窗口中返回非0  
     */
    int CloaseDialog(const QString& szId);

    int GetCount(){return m_Frame.size();}

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void closeEvent(QCloseEvent *);

private slots:
    void slotCloseTable(int nIndex);
    //刷新控件  
    void slotRefresh();
private:
    Ui::CFrmContainer *ui;
    QMap<QString, QSharedPointer<QFrame> >m_Frame;
    QTabWidget m_tabWidget;
    int m_nSize;//容纳子窗口大小  
};

#endif // FRMCONTAINER_H
