#include "FrmGroupChat.h"
#include "ui_FrmGroupChat.h"
#include "../../Global/Global.h"
#include <QMessageBox>
#include "../../MainWindow.h"
#include "../FrmUservCard/FrmUservCard.h"
#include <QDesktopWidget>

#ifdef WIN32
#undef SendMessage
#undef GetMessage
#endif

CFrmGroupChat::CFrmGroupChat(const QString &szId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChat)
{
    ui->setupUi(this);
    ui->txtInput->setFocus();//设置焦点  
    //设置过滤器,用于过滤发送消息快捷键,在 eventFilter 中处理  
    ui->txtInput->installEventFilter(this);

#ifdef MOBILE
    ui->lstMembers->setVisible(false);
    ui->pbMember->setVisible(true);
    ui->pbClose->setText(tr("Back"));
    ui->pbClose->setIcon(QIcon(":/icon/Left"));
#else
    ui->lstMembers->setVisible(true);
    ui->pbMember->setVisible(false);
    ui->pbSend->setMenu(&m_MessageSendMenu);
    ui->pbSend->setPopupMode(QToolButton::MenuButtonPopup);
#endif

    ui->lstMembers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pModelMembers = new QStandardItemModel(this);
    if(m_pModelMembers)
    {
        ui->lstMembers->setModel(m_pModelMembers);
    }

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);

    QSharedPointer<CManageGroupChat> mgc = GETMANAGER->GetManageGroupChat();
    m_Room = mgc->Get(szId);
    //设置标题  
    QString szTitle = m_Room->ShowName();
    if(!m_Room->Subject().isEmpty())
        szTitle += ":" + m_Room->Subject();
    this->setWindowTitle(szTitle);
    ui->lbTitle->setText(szTitle);

    //设置成员列表  
    foreach(QString member, m_Room->Participants())
    {
        
    }
}

CFrmGroupChat::~CFrmGroupChat()
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::~CFrmGroupChat");

    if(m_pModelMembers)
        delete m_pModelMembers;
    delete ui;
}

bool CFrmGroupChat::eventFilter(QObject *target, QEvent *event)
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

void CFrmGroupChat::on_pbSend_clicked()
{
    QString msg = ui->txtInput->toPlainText();
    if(msg.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("There is empty, please input."));
        return;
    }

    //AppendMessageToList(msg, m_Room->Nick());//服务器会转发，所以不需要  
    m_Room->SendMessage(msg);
    ui->txtInput->clear();//清空输入框中的内容  
}

void CFrmGroupChat::on_pbClose_clicked()
{
    close();
}

int CFrmGroupChat::AppendMessageToList(const QString &szMessage, const QString &nick)
{
    QString recMsg = szMessage;
    QString msg;
//    if(bRemote)
//        msg += "<p align='left'>";
//    else
//        msg += "<p align='right'>";
   // msg += "<img src='";
   // msg += CGlobal::Instance()->GetFileUserAvatar(nick) + "' width='16' height='16'>";
    msg += "<font color='";
    if(m_Room->Nick() != nick)
        msg += CGlobal::Instance()->GetRosterColor().name();
    else
        msg += CGlobal::Instance()->GetUserColor().name();
    msg += "'>[";
    msg += QTime::currentTime().toString() + "]" + nick + ":</font><br /><font color='";
    if(m_Room->Nick() != nick)
        msg += CGlobal::Instance()->GetRosterMessageColor().name();
    else
        msg += CGlobal::Instance()->GetUserMessageColor().name();
    msg += "'>";
    msg += recMsg.replace(QString("\n"), QString("<br />"));
    msg += "</font>";
    ui->txtView->append(msg);
    
    if(this->isHidden())
    {
        CGlobal::Instance()->GetMainWindow()->ShowTrayIconMessage(nick, szMessage);
    }
    
    return 0;
}

int CFrmGroupChat::ChangeTitle()
{
    QString szTitle = tr("Group chat [%1]:%2").arg(m_Room->ShowName(), m_Room->Subject());
    this->setWindowTitle(szTitle);
    ui->lbTitle->setText(szTitle);
    return 0;
}

void CFrmGroupChat::showEvent(QShowEvent *)
{
}

void CFrmGroupChat::closeEvent(QCloseEvent *)
{
    emit sigClose(this);
}

void CFrmGroupChat::resizeEvent(QResizeEvent *e)
{
}

void CFrmGroupChat::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CFrmGroupChat::on_pbMember_clicked()
{
    if(ui->lstMembers->isVisible())
    {
        ui->pbMember->setText(tr("Members(&M)"));
        ui->lstMembers->setVisible(false);
        ui->wdgChat->setVisible(true);
    }
    else
    {
        ui->pbMember->setText(tr("Chat(&C)"));
        ui->lstMembers->setVisible(true);
        ui->wdgChat->setVisible(false);
    }
}

void CFrmGroupChat::on_lstMembers_clicked(const QModelIndex &index)
{
#ifdef MOBILE
    on_lstMembers_doubleClicked(index);
#endif
}

void CFrmGroupChat::on_lstMembers_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_JID);
    QString jid = v.value<QString>();
    if(jid.isEmpty())
        return;
    //if(USER_INFO_LOCALE->GetBareJid() != QXmppUtils::jidToBareJid(jid))
    //    CFrmUservCard* pvCard = new CFrmUservCard(jid);
}
