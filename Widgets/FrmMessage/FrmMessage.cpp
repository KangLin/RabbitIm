#include "FrmMessage.h"
#include "ui_FrmMessage.h"
#include "../FrmUservCard/FrmUservCard.h"
#include "MainWindow.h"
#include "Global/Global.h"
#include "Message/style.h"
#include "Message/SmileyPack.h"
#include "Message/EmoticonsWidget.h"
#include "FileTransfer/ManageFileTransfer.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QDropEvent>
#include <QUrl>

#ifdef WIN32
#undef SendMessage
#undef GetMessage
#endif

CFrmMessage::CFrmMessage(QWidget *parent, Qt::WindowFlags f) :
    QFrame(parent, f),
    m_MessageSendMenu(parent),
    ui(new Ui::CFrmMessage)
{
    Init();
}

CFrmMessage::CFrmMessage(const QString &szId, QWidget *parent, Qt::WindowFlags f):
    QFrame(parent, f),
    m_MessageSendMenu(parent),
    ui(new Ui::CFrmMessage)
{
    Init(szId);
}

CFrmMessage::~CFrmMessage()
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::~CFrmMessage");
    CGlobal::Instance()->GetMainWindow()->disconnect(this);
    delete ui;
}

int CFrmMessage::Init(const QString &szId)
{
    bool check = false;
    ui->setupUi(this);
    ui->txtInput->setFocus();//设置焦点  
    ui->txtInput->installEventFilter(this);
    ui->txtView->setUndoRedoEnabled(false);
    ui->txtView->setAcceptRichText(false);
    ui->txtView->setOpenExternalLinks(false);
    ui->txtView->setOpenLinks(false);
#ifdef MOBILE
    ui->pbEmoticons->setText("");
#endif
    check = connect(ui->txtView, SIGNAL(anchorClicked(const QUrl &)),
                    SLOT(slotAnchorClicked(const QUrl &)));
    Q_ASSERT(check);

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

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotRefresh()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateRosterUserInfo(QString,QSharedPointer<CUser>)),
                    SLOT(slotUpdateRoster(QString,QSharedPointer<CUser>)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigChangedStatus(const QString&)),
                    SLOT(SlotChangedStatus(const QString&)));
    Q_ASSERT(check);

    QAction* pAction = m_MoreMenu.addAction(tr("send file"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotSendFile()));
    Q_ASSERT(check);
/*
    pAction = m_MoreMenu.addAction(tr("shot screen"));
    check = connect(pAction, SIGNAL(triggered()), SLOT(slotShotScreenTriggered()));
    Q_ASSERT(check);
    */
    ui->tbMore->setMenu(&m_MoreMenu);

    QDesktopWidget *pDesk = QApplication::desktop();    
#ifdef MOBILE
    this->resize(pDesk->geometry().size());
#else
    move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);
#endif 
    return 0;
}

void CFrmMessage::ChangedPresence(CUserInfo::USER_INFO_STATUS status)
{
    QPixmap pixmap;
    pixmap.convertFromImage(m_User->GetInfo()->GetPhoto());
    ui->lbAvatar->setPixmap(pixmap);

    ui->lbRosterName->setText(m_User->GetInfo()->GetShowName()
                              + "["
                              + CGlobal::Instance()->GetRosterStatusText(status)
                              + "]");
}

void CFrmMessage::slotSendFile()
{
    QString szId = m_User->GetInfo()->GetId();
    if(szId.isEmpty())
        return;
    
#ifdef MOBILE
    QDesktopWidget *pDesk = QApplication::desktop();
    QFileDialog dlg(pDesk, tr("Open File"));
    QScreen* pScreen = QApplication::primaryScreen();
    dlg.setGeometry(pScreen->availableGeometry());
    QString szFile;
    QStringList fileNames;
    if(dlg.exec())
        fileNames = dlg.selectedFiles();
    else
        return;
    if(fileNames.isEmpty())
        return;
    szFile = *fileNames.begin();
#else
    QString szFile = QFileDialog::getOpenFileName(
                this, tr("Open File"), 
                QString(), QString(), 0,
                QFileDialog::ReadOnly | QFileDialog::DontUseNativeDialog);
#endif
    
    QSharedPointer<CManageFileTransfer> file = CGlobal::Instance()->GetManager()->GetFileTransfer();
    file->SendFile(szId, szFile);
}
/*
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
    LOG_MODEL_DEBUG("Message", "CFrmMessage::closeEvent");
    if(GETMANAGER->GetFileTransfer()->GetFileTransfers(m_User->GetInfo()->GetId()))
    {
        QMessageBox msg(QMessageBox::Question,
                        tr("Close message dialog"),
                        tr("Sending files to determine whether you want to close?"),
                        QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::No == msg.exec())
        {
            e->ignore();
            return;
        }
        GETMANAGER->GetFileTransfer()->CancelSend(m_User->GetInfo()->GetId());
    }
    emit sigClose(this);
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

void CFrmMessage::dropEvent(QDropEvent *event)
{
    LOG_MODEL_DEBUG("CFrmMessage", "CFrmMessage::dropEvent");
}

bool CFrmMessage::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->txtInput) { 
        LOG_MODEL_DEBUG("CFrmMessage", "CFrmMessage::eventFilter event type:%d", event->type());
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
        else if(event->type() == QEvent::Drop)
        {
            LOG_MODEL_DEBUG("CFrmMessage", "CFrmMessage::eventFilter dropEvent");
            QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
            if(dropEvent->mimeData()->hasUrls())
            {
                foreach (QUrl url, dropEvent->mimeData()->urls()) {
                    QString szId = m_User->GetInfo()->GetId();
                    if(szId.isEmpty())
                        break;
                    QSharedPointer<CManageFileTransfer> file = CGlobal::Instance()->GetManager()->GetFileTransfer();
                    file->SendFile(szId, url.toLocalFile());
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
    if(m_User.isNull())
    {
        return;
    }

    slotRefresh();
    //通知未读数改变  
    slotMessageUpdate(m_User->GetInfo()->GetId());
    //设置焦点  
    ui->txtInput->setFocus();
}

void CFrmMessage::on_pbBack_clicked()
{
    close();
}

int CFrmMessage::AppendMessageToOutputView(std::vector<QSharedPointer<CChatAction> > action)
{
    for(auto it : action)
     {
         QTextCursor cursor = ui->txtView->textCursor();
         cursor.movePosition(QTextCursor::End);
         cursor.setKeepPositionOnInsert(true);
         ui->txtView->append(it->getContent());
         it->setup(cursor, ui->txtView);
    }
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

    //发送  
    GET_CLIENT->SendMessage(m_User->GetInfo()->GetId(),
                                          ui->txtInput->toPlainText());

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
        CFrmUservCard *pvCard = new CFrmUservCard(m_User->GetInfo());
        pvCard->show();
    }
}

void CFrmMessage::slotRefresh()
{
    if(m_User.isNull())
    {
        return;
    }

    ui->lbRosterName->setText(m_User->GetInfo()->GetShowName()
                              + "["
                              + CGlobal::Instance()->GetRosterStatusText(m_User->GetInfo()->GetStatus())
                              + "]");

   QPixmap pixmap;
   pixmap.convertFromImage(m_User->GetInfo()->GetPhoto());
   ui->lbAvatar->setPixmap(pixmap);
}

void CFrmMessage::slotMessageUpdate(const QString &szId)
{
    if(m_User.isNull() || m_User->GetInfo()->GetId() != szId)
        return;
    //通知未读数改变  
    std::vector<QSharedPointer<CChatAction> > msg = m_User->GetMessage()->GetUnreadMessage();
    AppendMessageToOutputView(msg);
    CGlobal::Instance()->GetManager()->GetRecentMessage()->slotMessageClean(szId);
    emit GET_CLIENT->sigMessageClean(m_User->GetInfo()->GetId());
}

void CFrmMessage::slotUpdateRoster(const QString &szId, QSharedPointer<CUser> user)
{
    if(m_User.isNull() || m_User->GetInfo()->GetId() != szId)
        return;
    Q_UNUSED(user);
    slotRefresh();
}

void CFrmMessage::SlotChangedStatus(const QString &szId)
{
    if(m_User.isNull() || m_User->GetInfo()->GetId() != szId)
        return;
    slotRefresh();
}

void CFrmMessage::slotAnchorClicked(const QUrl &url)
{
    LOG_MODEL_DEBUG("CFrmMessage", "CFrmMessage::slotAnchorClicked:scheme:%s;host:%s;query:%s",
                    qPrintable(url.scheme()),
                    qPrintable(url.host()),
                    qPrintable(url.query()));
    if(url.scheme() == "rabbitim")
    {
        if(url.host() == "userinfo")
        {
            //响应"<a href='rabbitim://userinfo'>";  
            this->on_lbAvatar_clicked();
        }
        else if(url.host() == "filetransfer")
        {
            GETMANAGER->GetFileTransfer()->ProcessCommand(m_User->GetInfo()->GetId(), url.query());
        }
    }
    else
     {
        QDesktopServices::openUrl(url);
    }
}

void CFrmMessage::on_pbEmoticons_clicked()
{
    if (CSmileyPack::getInstance().getEmoticons().empty())
        return;

    CEmoticonsWidget widget;
    connect(&widget, SIGNAL(insertEmoticon(QString)), this, SLOT(slotEmoteInsertRequested(QString)));

    QWidget* sender = qobject_cast<QWidget*>(QObject::sender());
    if (sender)
    {
        QPoint pos = -QPoint(widget.sizeHint().width() / 2, widget.sizeHint().height()) - QPoint(0, 10);
        widget.exec(sender->mapToGlobal(pos));
    }
}

void CFrmMessage::slotEmoteInsertRequested(const QString &s)
{
    ui->txtInput->insertPlainText(s);
    ui->txtInput->setFocus();
}
