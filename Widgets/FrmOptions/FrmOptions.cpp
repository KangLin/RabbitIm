#include "FrmOptions.h"
#include "ui_FrmOptions.h"
#include "../../Global.h"

CFrmOptions::CFrmOptions(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmOptions)
{
    ui->setupUi(this);
    m_pParent = parent;
    
    ui->cbNotificationFlash->setChecked(g_Global.IsNotifiationFlashs());
    ui->cbNotificationShowMessage->setChecked(g_Global.IsNotifiationBarShowMessage());
}

CFrmOptions::~CFrmOptions()
{
    delete ui;
}

CFrmOptions* CFrmOptions::Instance()
{
    static CFrmOptions* p = NULL;
    if(!p)
        p = new CFrmOptions;

    return p;
}

void CFrmOptions::on_pbCancel_clicked()
{
    close();
}

void CFrmOptions::on_pbOK_clicked()
{
    g_Global.SetNotifiationBarShowMessage(ui->cbNotificationShowMessage->isChecked());
    g_Global.SetNotifiationFlashs(ui->cbNotificationFlash->isChecked());

    close();
}
