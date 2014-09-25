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

    int ShowDialog(const QString& szId);
    int CloaseDialog(const QString& szId);

    int GetCount(){return m_Frame.size();}

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void closeEvent(QCloseEvent *);

private:
    Ui::CFrmContainer *ui;
    QMap<QString, QSharedPointer<QFrame> >m_Frame;
    QTabWidget m_tabWidget;
    int m_nSize;//容纳子窗口大小  
};

#endif // FRMCONTAINER_H
