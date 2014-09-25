#include "FrmMessage.h"
#include "ui_FrmMessage.h"
#include "../FrmUservCard/FrmUservCard.h"
#include "Global/Global.h"
#include <QKeyEvent>
#include <QMessageBox>

CFrmMessage::CFrmMessage(QWidget *parent) :
    QFrame(parent),
    m_MessageSendMenu(parent),
    ui(new Ui::CFrmMessage)
{
    Init();
}

CFrmMessage::CFrmMessage(const QString &szId, QWidget *parent):
    m_MessageSendMenu(parent),
    ui(new Ui::CFrmMessage)
{
    Init(szId);
}

CFrmMessage::~CFrmMessage()
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::~CFrmMessage");
    delete ui;
}

int CFrmMessage::Init(const QString &szId)
{
    ui->setupUi(this);
    ui->txtInput->setFocus();//设置焦点  
    ui->txtInput->installEventFilter(this);

    bool check = false;

    m_User = GLOBAL_USER->GetUserInfoRoster(szId);
    if(m_User.isNull())
    {
        LOG_MODEL_ERROR("CFrmMessage", "roster[%s] isn't exist.", szId.toStdString().c_str());
        return -1;
    }

#ifndef MOBILE
    ui->pbSend->setMenu(&m_MessageSendMenu);
    ui->pbSend->setPopupMode(QToolButton::MenuButtonPopup);
#endif

    /*/发送文件信号连接20140710 
    QAction* pAction = m_MoreMenu.addAction(tr("send file"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotSendFileTriggered()));
    Q_ASSERT(check);

    pAction = m_MoreMenu.addAction(tr("shot screen"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotShotScreenTriggered()));
    Q_ASSERT(check);
    */
    ui->tbMore->setMenu(&m_MoreMenu);
    return 0;
}

void CFrmMessage::ChangedPresence(CUserInfo::USER_INFO_STATUS status)
{
    QPixmap pixmap;
    pixmap.convertFromImage(m_User->GetPhoto());
    ui->lbAvatar->setPixmap(pixmap);

    ui->lbRosterName->setText(m_User->GetShowName()
                              + "["
                              + CGlobal::Instance()->GetRosterStatusText(status)
                              + "]");
}

/*
void CFrmMessage::slotSendFileTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "F:",
                                                    tr("All (*.*)"));
    if(fileName.isEmpty())
    {
        return;
    }
    if(m_pRoster->Resouce().isEmpty())
    {
        QMessageBox::critical(this, tr("Video"), tr("%1 isn't online.").arg(m_pRoster->ShowName()));
        return;
    }
    CGlobal::Instance()->GetMainWindow()->sendFile(m_pRoster->Jid(),fileName);
}

void CFrmMessage::slotShotScreenTriggered()
{
    CDlgScreenShot dlg;
    if(dlg.exec() ==  QDialog::Accepted)
    {
        QImage image = dlg.getSelectedImg().toImage();
        CGlobal::E_SCREEN_SHOT_TO_TYPE type = CGlobal::Instance()->GetScreenShotToType();
        if(type == CGlobal::E_TO_CLIPBOARD)
        {
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setImage(image);//参数是否合适TODO  
        }
        else if(type == CGlobal::E_TO_SAVE)
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                       "./untitled.png",
                                       tr("Images (*.png *.xpm *.jpg)"));
            if(!fileName.isEmpty())
            {
                bool isOk = image.save(fileName);
                if(!isOk)
                {
                    LOG_MODEL_ERROR("Message", "save file [%s] is error", fileName.toStdString().c_str());
                }
            }
        }
        else
        {
            QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss.png");
            QString filePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +QDir::separator() +  fileName;
            LOG_MODEL_DEBUG("Message", QString("filePath = %1").arg(filePath).toLocal8Bit().data());
            bool isOk = image.save(filePath);
            if(isOk)
            {
                if(m_pRoster->Resouce().isEmpty())
                {
                    QMessageBox::critical(this, tr("Video"), tr("%1 isn't online.").arg(m_pRoster->ShowName()));
                    return;
                }
                CGlobal::Instance()->GetMainWindow()->sendFile(m_pRoster->Jid(), filePath, MainWindow::ImageType);
            }
            else
            {
                LOG_MODEL_ERROR("Message", "save file [%s] is error", filePath.toStdString().c_str());
            }
        }
    }
}
*/
void CFrmMessage::hideEvent(QHideEvent *)
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::hideEvent");
}

void CFrmMessage::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Message", "CFrmMessage::closeEvent");
}

void CFrmMessage::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

bool CFrmMessage::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->txtInput) { 
        if (event->type() == QEvent::KeyPress) { 
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event); 
            CGlobal::E_MESSAGE_SEND_TYPE type = CGlobal::Instance()->GetMessageSendType();
            if(CGlobal::E_MESSAGE_SEND_TYPE_CTRL_ENTER == type)
            {
                if(keyEvent->key() == Qt::Key_Return
                   && (keyEvent->modifiers() & Qt::ControlModifier))
                {
                    this->on_pbSend_clicked();
                    return true;
                }
            }
            else
            {
                if (keyEvent->key() == Qt::Key_Return) {
                    this->on_pbSend_clicked();
                    return true; 
                }
            }
        }
    }
    return QFrame::eventFilter(target, event);
}

void CFrmMessage::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::showEvent");
    //TODO:重读数据  
    if(!m_User.isNull())
    {
        return;
    }

    ui->lbRosterName->setText(m_User->GetShowName()
                              + "["
                              + CGlobal::Instance()->GetRosterStatusText(m_User->GetStatus())
                              + "]");

    m_User->SetUnReadMessageCount(0);
}

void CFrmMessage::on_pbBack_clicked()
{
    close();
}

int CFrmMessage::AppendMessageToList(const QString &szMessage, const QString &bareJid, const QString &name, bool bRemote)
{
    QString recMsg = szMessage;
    QString msg;
//    if(bRemote)
//        msg += "<p align='left'>";
//    else
//        msg += "<p align='right'>";
    msg += "<img src='";
    msg += CGlobal::Instance()->GetFileUserAvatar(bareJid) + "' width='16' height='16'>";
    msg += "<font color='";
    if(bRemote)
        msg += CGlobal::Instance()->GetRosterColor().name();
    else
        msg += CGlobal::Instance()->GetUserColor().name();
    msg += "'>[";
    msg += QTime::currentTime().toString()  +  "]" + name +  ":</font><br /><font color='";
    if(bRemote)
        msg += CGlobal::Instance()->GetRosterMessageColor().name();
    else
        msg += CGlobal::Instance()->GetUserMessageColor().name();
    msg += "'>";
    msg += recMsg.replace(QString("\n"), QString("<br />"));
    msg += "</font>";
    //LOG_MODEL_DEBUG("message", "html:%s", msg.toStdString().c_str());
    ui->txtView->append(msg);
    return 0;
}

int CFrmMessage::AppendMessage(const QString &szMessage)
{
    if(!this->isHidden())
        this->activateWindow();

    AppendMessageToList(szMessage, m_User->GetId(), m_User->GetShowName(), true);
    return 0;
}

void CFrmMessage::on_pbSend_clicked()
{
    //QString message=ui->txtInput->toHtml();
    //LOG_MODEL_DEBUG("FrmMessage", "message:%s", message.toStdString().c_str());
    QString message = ui->txtInput->toPlainText();
    if(message.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("There is empty, please input."));
        return;
    }

    AppendMessageToList(ui->txtInput->toPlainText(), USER_INFO_LOCALE->GetId(), USER_INFO_LOCALE->GetShowName(), false);

    //发送  
    GET_CLIENT->SendMessage(m_User->GetId(), ui->txtInput->toPlainText());

    ui->txtInput->clear();//清空输入框中的内容  
}

void CFrmMessage::on_tbMore_clicked()
{
}

void CFrmMessage::on_pbVideo_clicked()
{
/*    if(m_pRoster->Resouce().isEmpty())
    {
        QMessageBox::critical(this, tr("Video"), tr("%1 isn't online.").arg(m_pRoster->ShowName()));
        return;
    }
    CFrmVideo *pVideo = CFrmVideo::instance();
    if(NULL == pVideo)
        return;

    QDesktopWidget *pDesk = QApplication::desktop();
    pVideo->move((pDesk->width() - pVideo->width()) / 2, (pDesk->height() - pVideo->height()) / 2);
    pVideo->show();
    pVideo->activateWindow();

    pVideo->Call(m_pRoster->Jid());*/
}

void CFrmMessage::on_lbAvatar_clicked()
{
    if(!m_User.isNull())
    {
        CFrmUservCard *pvCard = new CFrmUservCard(m_User);
        pvCard->show();
    }
}
