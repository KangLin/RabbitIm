#include "FrmOptions.h"
#include "ui_FrmOptions.h"
#include "../../Global/Global.h"
#include <QColorDialog>
#include "../../MainWindow.h"
#include <QDesktopWidget>

CFrmOptions::CFrmOptions(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmOptions)
{
    ui->setupUi(this);

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);

    m_pParent = parent;

#ifdef MOBILE
    ui->gBSendMessage->setVisible(false);
#endif

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
    CGlobal::Instance()->GetMainWindow()->setEnabled(false);
    ui->cbAutoLogin->setChecked(CGlobal::Instance()->GetAutoLogin());
    ui->sbDelayTime->setValue(CGlobal::Instance()->GetAutoLoginDelayTime());
    ui->cbNotificationFlash->setChecked(CGlobal::Instance()->IsNotifiationFlashs());
    ui->cbNotificationShowMessage->setChecked(CGlobal::Instance()->IsNotifiationBarShowMessage());
    ui->txtFlashInterval->setText(QString::number(CGlobal::Instance()->GetNotifiationFlashInterval()));
    ui->txtShowMessageDelay->setText(QString::number(CGlobal::Instance()->GetNotifiationBarShowMessageDelay()));
    ui->cbMessageSound->setChecked(CGlobal::Instance()->GetMessageSound());

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
    
    CGlobal::E_CLOSE_TYPE closeType = CGlobal::Instance()->GetCloseType();
    switch(closeType)
    {
    case CGlobal::E_CLOSE_TYPE_NO:
        ui->tBtnPrompt->setChecked(true);
        break;
    case CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME:
        ui->rBtnClosePrograme->setChecked(true);
        break;
    case CGlobal::E_CLOSE_TYPE_LOGOUT:
        ui->rBtnLogout->setChecked(true);
        break;
    }

    CGlobal::E_MESSAGE_SEND_TYPE messageType = CGlobal::Instance()->GetMessageSendType();
    switch(messageType){
    case CGlobal::E_MESSAGE_SEND_TYPE_ENTER:
        ui->rbtnENTER->setChecked(true);
        break;
    case CGlobal::E_MESSAGE_SEND_TYPE_CTRL_ENTER:
    default:
        ui->rBtnCTRLENTER->setChecked(true);
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
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUnreadMessageCountColor());
    ui->lbUnreadMessageCountColor->setPalette(pa);

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

void CFrmOptions::closeEvent(QCloseEvent *)
{
    CGlobal::Instance()->GetMainWindow()->setEnabled(true);
}

void CFrmOptions::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
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
    CGlobal::Instance()->SetMessageSound(ui->cbMessageSound->isChecked());

    CGlobal::E_ROSTER_SHOW_TYPE type = CGlobal::Instance()->GetRosterShowType();
    if(ui->rbJID->isChecked())
        type = CGlobal::E_ROSTER_SHOW_JID;
    else if(ui->rbName->isChecked())
        type = CGlobal::E_ROSTER_SHOW_NAME;
    else //if(ui->rbNick->isChecked())
        type = CGlobal::E_ROSTER_SHOW_NICK;
    CGlobal::Instance()->SetRosterShowType(type);
    
    CGlobal::E_CLOSE_TYPE closeType = CGlobal::Instance()->GetCloseType();
    if(ui->rBtnClosePrograme->isChecked())
        closeType = CGlobal::E_CLOSE_TYPE_CLOSE_PROGRAME;
    else if(ui->rBtnLogout->isChecked())
        closeType = CGlobal::E_CLOSE_TYPE_LOGOUT;
    else if(ui->tBtnPrompt->isChecked())
        closeType = CGlobal::E_CLOSE_TYPE_NO;
    CGlobal::Instance()->SetCloseType(closeType);

    CGlobal::E_MESSAGE_SEND_TYPE messageType = CGlobal::Instance()->GetMessageSendType();
    if(ui->rbtnENTER->isChecked())
        messageType = CGlobal::E_MESSAGE_SEND_TYPE_ENTER;
    else if(ui->rBtnCTRLENTER->isChecked())
        messageType = CGlobal::E_MESSAGE_SEND_TYPE_CTRL_ENTER;
    CGlobal::Instance()->SetMessageSendType(messageType);

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

void CFrmOptions::on_pbUnreadMessageCountColor_clicked()
{
    CGlobal::Instance()->SetUnreadMessageCountColor(QColorDialog::getColor(CGlobal::Instance()->GetUnreadMessageCountColor()));
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUnreadMessageCountColor());
    ui->lbUnreadMessageCountColor->setPalette(pa);
}
