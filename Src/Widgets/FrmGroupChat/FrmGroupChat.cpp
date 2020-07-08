#include "FrmGroupChat.h"
#include "ui_FrmGroupChat.h"
#include "../../Global/Global.h"
#include <QMessageBox>
#include "../../MainWindow.h"
#include "../DlgUservCard/DlgUservCard.h"
#include <QDesktopWidget>
#include "ChatActionGroupChat.h"

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
        AddMember(member);
    }

    //初始化连接  
    bool check = connect(m_Room.data(), SIGNAL(sigParticipantAdd(QString)),
                         SLOT(slotParticipantAdd(QString)));
    Q_ASSERT(check);
    check = connect(m_Room.data(), SIGNAL(sigParticipantRemoved(QString)),
                    SLOT(slotParticipantRemoved(QString)));
    Q_ASSERT(check);
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigUpdateMessage(const QString&)),
                    this,
                    SLOT(slotUpdateMessage(const QString&)));
    Q_ASSERT(check);
    check = connect(ui->lstMembers, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotMemberCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);
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

int CFrmGroupChat::ChangeTitle()
{
    QString szTitle = tr("Group chat [%1]:%2").arg(m_Room->ShowName(), m_Room->Subject());
    this->setWindowTitle(szTitle);
    ui->lbTitle->setText(szTitle);
    return 0;
}

void CFrmGroupChat::showEvent(QShowEvent *)
{
    slotUpdateMessage(m_Room->Id());
}

void CFrmGroupChat::closeEvent(QCloseEvent *)
{
    emit sigClose(this);
}

void CFrmGroupChat::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
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
    Q_UNUSED(index);
#ifdef MOBILE
    //on_lstMembers_doubleClicked(index);
#endif
}

void CFrmGroupChat::on_lstMembers_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_JID);
    QString szId = v.value<QString>();
    if(szId.isEmpty())
        return;
     szId = m_Room->ParticipantId(szId);
    if(USER_INFO_LOCALE->GetInfo()->GetId() == szId)
        return;

    CDlgUservCard pvCard(szId);
    pvCard.exec();
}

void CFrmGroupChat::slotParticipantAdd(const QString &szId)
{
    LOG_MODEL_DEBUG("CFrmGroupChat", "CFrmGroupChat::slotParticipantAdd:%s", qPrintable(szId));
    AddMember(szId);
}

void CFrmGroupChat::slotParticipantRemoved(const QString &szId)
{
    RemoveMember(szId);
}

void CFrmGroupChat::slotMemberCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QModelIndex index = ui->lstMembers->currentIndex();
    if(!index.isValid())
        return;
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_JID);
    QString szId = v.value<QString>();
    if(szId.isEmpty())
        return;
    if(USER_INFO_LOCALE->GetInfo()->GetId() == m_Room->ParticipantId(szId))
        return;
    QMenu menu;
    if(m_Room->Affiliation(szId) == CGroupChat::AdminAffiliation 
            || m_Room->Affiliation(szId) == CGroupChat::OwnerAffiliation)
        menu.addAction(ui->actionMember_kick);
    menu.addAction(ui->actionMember_information);
    menu.exec(QCursor::pos());
}

void CFrmGroupChat::slotUpdateMessage(const QString &szId)
{
    if(szId != m_Room->Id())
        return;
    //通知未读数改变  
    std::vector<QSharedPointer<CChatAction> > msg = m_Room->GetMessage()->GetUnreadMessage();
    if(msg.size() == 0)
        return;
    AppendMessageToOutputView(msg);
    CGlobal::Instance()->GetManager()->GetRecentMessage()->slotMessageClean(szId);
    emit GETMANAGER->GetManageGroupChat()->sigMessageClean(m_Room->Id());
}

int CFrmGroupChat::AddMember(const QString &szId)
{
    QStandardItem* pItem = new QStandardItem(m_Room->ParticipantNick(szId));
    pItem->setData(szId, Qt::ToolTipRole);
    pItem->setData(szId, ROLE_GROUPCHAT_JID);
    m_pModelMembers->appendRow(pItem);
    return 0;
}

int CFrmGroupChat::RemoveMember(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModelMembers->match(m_pModelMembers->index(0, 0),
                                                ROLE_GROUPCHAT_JID, 
                                                szId, 
                                                -1,
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    QModelIndex index;
    foreach(index, lstIndexs)
    {
        m_pModelMembers->removeRow(index.row());
        bool isMe = m_Room->ParticipantId(szId) == USER_INFO_LOCALE->GetInfo()->GetId();
        if(isMe)
        {
            this->close();
            emit GETMANAGER->GetManageGroupChat()->sigLeave(m_Room->Id());
            return 0;
        }
        //TODO:只有所有者和管理员才接收下面消息  
        QSharedPointer<CChatActionGroupChat> action(
                    new CChatActionGroupChat(
                        m_Room->Id(),
                        tr("Leave the group chat"),
                        szId,
                        QTime::currentTime(), 
                        isMe
                        )
                    );
        m_Room->GetMessage()->AddMessage(action);
        emit GETMANAGER->GetManageGroupChat()->sigUpdateMessage(m_Room->Id());
    }
    return 0;
}

int CFrmGroupChat::AppendMessageToOutputView(std::vector<QSharedPointer<CChatAction> > action)
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

void CFrmGroupChat::on_actionMember_information_triggered()
{
    QModelIndex index = ui->lstMembers->currentIndex();
    if(!index.isValid())
        return;
    on_lstMembers_doubleClicked(index);
}

void CFrmGroupChat::on_actionMember_kick_triggered()
{
    QModelIndex index = ui->lstMembers->currentIndex();
    if(!index.isValid())
        return;
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_JID);
    QString szId = v.value<QString>();
    if(szId.isEmpty())
        return;
    
    if(USER_INFO_LOCALE->GetInfo()->GetId() != m_Room->ParticipantId(szId))
        m_Room->Kick(szId);
}
