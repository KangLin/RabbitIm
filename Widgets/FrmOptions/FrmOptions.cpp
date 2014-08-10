#include "FrmOptions.h"
#include "ui_FrmOptions.h"
#include "../../Global.h"
#include <QColorDialog>

CFrmOptions::CFrmOptions(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmOptions)
{
    ui->setupUi(this);
    m_pParent = parent;

    //控件初始化工作放到showEvent中  
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

void CFrmOptions::showEvent(QShowEvent *)
{
    ui->cbAutoLogin->setChecked(CGlobal::Instance()->GetAutoLogin());
    ui->sbDelayTime->setValue(CGlobal::Instance()->GetAutoLoginDelayTime());
    ui->cbNotificationFlash->setChecked(CGlobal::Instance()->IsNotifiationFlashs());
    ui->cbNotificationShowMessage->setChecked(CGlobal::Instance()->IsNotifiationBarShowMessage());
    ui->txtFlashInterval->setText(QString::number(CGlobal::Instance()->GetNotifiationFlashInterval()));
    ui->txtShowMessageDelay->setText(QString::number(CGlobal::Instance()->GetNotifiationBarShowMessageDelay()));
    
    CGlobal::E_ROSTER_SHOW_TYPE type = CGlobal::Instance()->GetRosterShowType();
    switch (type) {
    case CGlobal::E_ROSTER_SHOW_JID:
        ui->rbJID->setChecked(true);
        break;
    case CGlobal::E_ROSTER_SHOW_NAME:
        ui->rbName->setChecked(true);
        break;
    case CGlobal::E_ROSTER_SHOW_NICK:
    default:
        ui->rbNick->setChecked(true);
        break;
    }
    
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserColor());
    ui->lbLocalUserColor->setPalette(pa);
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterColor());
    ui->lbRosterColor->setPalette(pa);
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterMessageColor());
    ui->lbRosterMessageColor->setPalette(pa);
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserMessageColor());
    ui->lbLocalUserMessageColor->setPalette(pa);

    CGlobal::E_SCREEN_SHOT_TO_TYPE screenShotType = CGlobal::Instance()->GetScreenShotToType();
    switch (screenShotType) {
    case CGlobal::E_TO_CLIPBOARD:
        ui->rBtn_clipboard->setChecked(true);
        break;
    case CGlobal::E_TO_SAVE:
        ui->rBtn_save->setChecked(true);
        break;
    case CGlobal::E_TO_SEND:
    default:
        ui->rBtn_send->setChecked(true);
        break;
    }
    
}

void CFrmOptions::on_pbCancel_clicked()
{
    close();
}

void CFrmOptions::on_pbOK_clicked()
{
    CGlobal::Instance()->SetAutoLogin(ui->cbAutoLogin->isChecked());
    CGlobal::Instance()->SetAutoLoginDelayTime(ui->sbDelayTime->value());
    CGlobal::Instance()->SetNotifiationBarShowMessage(ui->cbNotificationShowMessage->isChecked());
    CGlobal::Instance()->SetNotifiationFlashs(ui->cbNotificationFlash->isChecked());
    CGlobal::Instance()->SetNotifiationFlashInterval(ui->txtFlashInterval->text().toInt());
    CGlobal::Instance()->SetNotifiationBarShowMessageDelay(ui->txtShowMessageDelay->text().toInt());

    CGlobal::E_ROSTER_SHOW_TYPE type = CGlobal::Instance()->GetRosterShowType();
    if(ui->rbJID->isChecked())
        type = CGlobal::E_ROSTER_SHOW_JID;
    else if(ui->rbName->isChecked())
        type = CGlobal::E_ROSTER_SHOW_NAME;
    else //if(ui->rbNick->isChecked())
        type = CGlobal::E_ROSTER_SHOW_NICK;
    CGlobal::Instance()->SetRosterShowType(type);
    
    CGlobal::E_SCREEN_SHOT_TO_TYPE screenShotToType = CGlobal::Instance()->GetScreenShotToType();
    if(ui->rBtn_save->isChecked())
        screenShotToType = CGlobal::E_TO_SAVE;
    else if(ui->rBtn_clipboard->isChecked())
        screenShotToType = CGlobal::E_TO_CLIPBOARD;
    else
        screenShotToType = CGlobal::E_TO_SEND;
    
    CGlobal::Instance()->SetScreenShotToType(screenShotToType);

    emit sigRefresh();

    close();
}

void CFrmOptions::on_pbLocalUserColor_clicked()
{
    CGlobal::Instance()->SetUserColor(QColorDialog::getColor(CGlobal::Instance()->GetUserColor()));
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserColor());
    ui->lbLocalUserColor->setPalette(pa);
}

void CFrmOptions::on_pbRosterColor_clicked()
{
    CGlobal::Instance()->SetRosterColor(QColorDialog::getColor(CGlobal::Instance()->GetRosterColor()));
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterColor());
    ui->lbRosterColor->setPalette(pa);
}

void CFrmOptions::on_pbLocalUserMessageColor_clicked()
{
    CGlobal::Instance()->SetUserMessageColor(QColorDialog::getColor(CGlobal::Instance()->GetUserMessageColor()));
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserMessageColor());
    ui->lbLocalUserMessageColor->setPalette(pa);
}

void CFrmOptions::on_pbRosterMessageColor_clicked()
{
    CGlobal::Instance()->SetRosterMessageColor(QColorDialog::getColor(CGlobal::Instance()->GetRosterMessageColor()));
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterMessageColor());
    ui->lbRosterMessageColor->setPalette(pa);
}
