#ifndef FRMAPP_H
#define FRMAPP_H

#include <QWidget>
#include "FrmAppList.h"
#include "FrmFavorites.h"

namespace Ui {
class CFrmApp;
}

class CFrmApp : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmApp(QWidget *parent = 0);
    ~CFrmApp();
private:
    virtual void changeEvent(QEvent*);

private:
    Ui::CFrmApp *ui;
    CFrmAppList m_AppList;
    CFrmFavorites m_AppFavorites;
};

#endif // FRMAPP_H
