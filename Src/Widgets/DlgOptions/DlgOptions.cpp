#include "DlgOptions.h"
#include "ui_DlgOptions.h"
#include "../../Global/Global.h"
#include "../../MainWindow.h"
#include <QDesktopWidget>
#include <QColorDialog>
#include <QAudioInput>
#include <QAudioOutput>
#include <QDir>
#include <QCameraInfo>
#include "Tool.h"
#include "RabbitCommonDir.h"

CDlgOptions::CDlgOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgOptions)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    LOG_MODEL_DEBUG("CDlgOptions", "CDlgOptions::CDlgOptions:w:%d, h:%d",
                    this->width(), this->height());
    //控件初始化工作放到showEvent中  
    int nIndex = 0;
    if(USER_INFO_LOCALE.isNull())
    {
        QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                       QSettings::IniFormat);
        nIndex = conf.value("Widgets/Options", 0).toInt();
    }
    else
    {
        QSettings conf(CGlobalDir::Instance()->GetUserConfigureFile(
                   USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
        nIndex = conf.value("Widgets/Options", 0).toInt();
    }
    ui->tabWidget->setCurrentIndex(nIndex);
    
    ui->sbWindowDelayTime->setToolTip(tr("Set range: ")
                           + QString::number(ui->sbWindowDelayTime->minimum())
                           + " - "
                           + QString::number(ui->sbWindowDelayTime->maximum()));
}

CDlgOptions::~CDlgOptions()
{
    if(!USER_INFO_LOCALE.isNull())
    {
        QSettings conf(CGlobalDir::Instance()->GetUserConfigureFile(
                   USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
        conf.setValue("Widgets/Options", ui->tabWidget->currentIndex());
    }
    else
    {
        QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                       QSettings::IniFormat);
        conf.setValue("Widgets/Options", ui->tabWidget->currentIndex());
    }
    delete ui;
}

void CDlgOptions::showEvent(QShowEvent *)
{
    ui->cbAutoLogin->setChecked(CGlobal::Instance()->GetAutoLogin());
    ui->sbDelayTime->setValue(CGlobal::Instance()->GetAutoLoginDelayTime());
    ui->cbNotificationFlash->setChecked(CGlobal::Instance()->IsNotifiationFlashs());
    ui->cbNotificationShowMessage->setChecked(CGlobal::Instance()->IsNotifiationBarShowMessage());
    ui->sbFlashInterval->setValue(CGlobal::Instance()->GetNotifiationFlashInterval());
    ui->sbShowMessageDelay->setValue(CGlobal::Instance()->GetNotifiationBarShowMessageDelay());
    ui->cbMessageSound->setChecked(CGlobal::Instance()->GetMessageSound());
    ui->sbWindowDelayTime->setValue(CGlobal::Instance()->GetAnimationHideMainWindow());
    ui->sbAnimationDuration->setValue(CGlobal::Instance()->GetAnimationDuration());
    ui->cbAutoHideMainWindow->setChecked(CGlobal::Instance()->IsAnimationHideMainWindow());

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

    ui->cbVideo->addItem(tr("no device"));
    QList<QCameraInfo> info = QCameraInfo::availableCameras();
    foreach(auto i, info)
    {
        ui->cbVideo->addItem(i.description() + "[" + i.deviceName() + "]");
    }
    ui->cbVideo->setCurrentIndex(CGlobal::Instance()->GetVideoCaptureDevice() + 1);

    ui->cbAudioInput->addItem(tr("no device"));
    QList<QAudioDeviceInfo> infos = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo info, infos) {
        ui->cbAudioInput->addItem(info.deviceName());
    }
    ui->cbAudioOutput->addItem(tr("no device"));
    ui->cbAudioInput->setCurrentIndex(CGlobal::Instance()->GetAudioInputDevice() + 1);
    infos = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo info, infos) {
        ui->cbAudioOutput->addItem(info.deviceName());
    }
    ui->cbAudioOutput->setCurrentIndex(CGlobal::Instance()->GetAudioOutputDevice() + 1);

    ui->cbShowLocaleVideo->setChecked(CGlobal::Instance()->GetIsShowLocaleVideo());
    ui->cbMonitor->setChecked(CGlobal::Instance()->GetIsMonitor());
}

void CDlgOptions::closeEvent(QCloseEvent *)
{
}

void CDlgOptions::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgOptions::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::resizeEvent:e.size:%d;genmetry.size:%d,frame.size:%d",
                    e->size().width(),
                    geometry().size().width(),
                    this->frameGeometry().width());
}

void CDlgOptions::on_pbCancel_clicked()
{
    this->reject();
}

void CDlgOptions::on_pbOK_clicked()
{
    CGlobal::Instance()->SetAutoLogin(ui->cbAutoLogin->isChecked());
    CGlobal::Instance()->SetAutoLoginDelayTime(ui->sbDelayTime->value());
    CGlobal::Instance()->SetNotifiationBarShowMessage(ui->cbNotificationShowMessage->isChecked());
    CGlobal::Instance()->SetNotifiationFlashs(ui->cbNotificationFlash->isChecked());
    CGlobal::Instance()->SetNotifiationFlashInterval(ui->sbFlashInterval->value());
    CGlobal::Instance()->SetNotifiationBarShowMessageDelay(ui->sbShowMessageDelay->value());
    CGlobal::Instance()->SetMessageSound(ui->cbMessageSound->isChecked());
    CGlobal::Instance()->SetAnimationHideMainWindow(ui->sbWindowDelayTime->value());
    CGlobal::Instance()->SetAnimationDuration(ui->sbAnimationDuration->value());
    CGlobal::Instance()->SetIsAnimationHideMainWindow(ui->cbAutoHideMainWindow->isChecked());

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

    CGlobal::Instance()->SetVideoCaptureDevice(ui->cbVideo->currentIndex() - 1);
    CGlobal::Instance()->SetAudioInputDevice(ui->cbAudioInput->currentIndex() - 1);
    CGlobal::Instance()->SetAudioOutputDevice(ui->cbAudioOutput->currentIndex() - 1);

    CGlobal::Instance()->SetIsShowLocaleVideo(ui->cbShowLocaleVideo->isChecked());
    CGlobal::Instance()->SetMonitor(ui->cbMonitor->isChecked());

    emit GET_MAINWINDOW->sigRefresh();

    this->accept();
}

void CDlgOptions::on_pbLocalUserColor_clicked()
{
    QColor color;
    QColorDialog dlgColor(this);
    CTool::SetWindowsGeometry(&dlgColor);
    dlgColor.setCurrentColor(CGlobal::Instance()->GetUserColor());
    if(QDialog::Rejected == dlgColor.exec())
        return;
    color = dlgColor.currentColor();
    CGlobal::Instance()->SetUserColor(color);
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserColor());
    ui->lbLocalUserColor->setPalette(pa);
}

void CDlgOptions::on_pbRosterColor_clicked()
{
    QColor color;
    QColorDialog dlgColor(this);
    CTool::SetWindowsGeometry(&dlgColor);
    dlgColor.setCurrentColor(CGlobal::Instance()->GetUserColor());
    if(QDialog::Rejected == dlgColor.exec())
        return;
    color = dlgColor.currentColor();
    CGlobal::Instance()->SetRosterColor(color);
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterColor());
    ui->lbRosterColor->setPalette(pa);
}

void CDlgOptions::on_pbLocalUserMessageColor_clicked()
{
    QColor color;
    QColorDialog dlgColor(this);
    CTool::SetWindowsGeometry(&dlgColor);
    dlgColor.setCurrentColor(CGlobal::Instance()->GetUserColor());
    if(QDialog::Rejected == dlgColor.exec())
        return;
    color = dlgColor.currentColor();
    CGlobal::Instance()->SetUserMessageColor(color);
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUserMessageColor());
    ui->lbLocalUserMessageColor->setPalette(pa);
}

void CDlgOptions::on_pbRosterMessageColor_clicked()
{
    QColor color;
    QColorDialog dlgColor(this);
    CTool::SetWindowsGeometry(&dlgColor);
    dlgColor.setCurrentColor(CGlobal::Instance()->GetUserColor());
    if(QDialog::Rejected == dlgColor.exec())
        return;
    color = dlgColor.currentColor();
    CGlobal::Instance()->SetRosterMessageColor(color);
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetRosterMessageColor());
    ui->lbRosterMessageColor->setPalette(pa);
}

void CDlgOptions::on_pbUnreadMessageCountColor_clicked()
{
    QColor color;
    QColorDialog dlgColor(this);
    CTool::SetWindowsGeometry(&dlgColor);
    dlgColor.setCurrentColor(CGlobal::Instance()->GetUserColor());
    if(QDialog::Rejected == dlgColor.exec())
        return;
    color = dlgColor.currentColor();
    CGlobal::Instance()->SetUnreadMessageCountColor(color);
    QPalette pa;
    pa.setColor(QPalette::WindowText, CGlobal::Instance()->GetUnreadMessageCountColor());
    ui->lbUnreadMessageCountColor->setPalette(pa);
}

void CDlgOptions::on_pbResetConfigure_clicked()
{
    if(ui->cbConfigureFile->isChecked())
        CTool::removeDirectory(CGlobalDir::Instance()->GetDirUserConfigure());
    if(ui->cbDataFile->isChecked())
        CTool::removeDirectory(CGlobalDir::Instance()->GetDirUserData());
}
