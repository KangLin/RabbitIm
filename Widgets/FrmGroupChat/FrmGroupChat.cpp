#include "FrmGroupChat.h"
#include "ui_FrmGroupChat.h"
#include "../../Global.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppUtils.h"
#include <QMessageBox>
#include "../FrmUserList/Roster.h"

CFrmGroupChat::CFrmGroupChat(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChat)
{
    ui->setupUi(this);
    m_pRoom = NULL;
    m_pItem = NULL;

    ui->lstMembers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pModel = new QStandardItemModel(this);//这里不会产生内在泄漏，控件在romve操作时会自己释放内存。  
    if(m_pModel)
    {
        ui->lstMembers->setModel(m_pModel);
    }
}

CFrmGroupChat::~CFrmGroupChat()
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::~CFrmGroupChat");
    if(m_pRoom)
        delete m_pRoom;

    delete ui;
}

bool CFrmGroupChat::Join(const QString &jid)
{
    QList<QXmppMucRoom*> rooms = CGlobal::Instance()->GetXmppClient()->m_MucManager.rooms();
    QXmppMucRoom* r;
    foreach(r, rooms)
    {
        if(r->jid() == jid)
        {
            LOG_MODEL_DEBUG("Group chat", "had joined room[%s]", qPrintable(jid));
            return false;
        }
    }

    m_pRoom = CGlobal::Instance()->GetXmppClient()->m_MucManager.addRoom(jid);
    if(m_pRoom)
    {
        bool check = connect(m_pRoom, SIGNAL(joined()),
                             SLOT(slotJoined()));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(left()),
                        SLOT(slotLeft()));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(nameChanged(QString)), 
                        SLOT(slotNameChanged(QString)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(error(QXmppStanza::Error)),
                        SLOT(slotError(QXmppStanza::Error)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(subjectChanged(QString)),
                        SLOT(slotSubjectChanged(QString)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(messageReceived(QXmppMessage)),
                        SLOT(slotMessageReceived(QXmppMessage)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(participantAdded(QString)),
                        SLOT(slotParticipantAdded(QString)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(participantChanged(QString)),
                        SLOT(slotParticipantChanged(QString)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(participantRemoved(QString)),
                        SLOT(slotParticipantRemoved(QString)));
        Q_ASSERT(check);
        
        check = connect(m_pRoom, SIGNAL(permissionsReceived(QList<QXmppMucItem>)),
                        SLOT(slotPermissionsReceived(QList<QXmppMucItem>)));
        Q_ASSERT(check);
 
        check = connect(m_pRoom, SIGNAL(configurationReceived(QXmppDataForm)),
                        SLOT(slotConfigurationReceived(QXmppDataForm)));
        Q_ASSERT(check);

        //设置昵称  
        m_pRoom->setNickName(CGlobal::Instance()->GetRoster()->Name());
        //加入房间  
        return m_pRoom->join();
    }
    return false;
}

bool CFrmGroupChat::Create(const QString &jid)
{
    if(!this->Join(jid))
        return false;
    if(m_pRoom)
    {
        //m_pRoom->requestConfiguration();
        //正常流程是先请求配置，然后在请求配置信号中处理配置  
        //并发送设置配置  
        return m_pRoom->setConfiguration(m_DataForm);
    }
    return false;
}

bool CFrmGroupChat::Leave(const QString &message)
{
    if(m_pRoom)
        return m_pRoom->leave(message);
    return false;
}

bool CFrmGroupChat::setConfiguration(const QXmppDataForm &form)
{
    m_DataForm = form;
    return false;
}

QStandardItem* CFrmGroupChat::GetItem()
{
    if(NULL == m_pItem)
    {
        m_pItem = new QStandardItem(QIcon(":/icon/Conference"), m_pRoom->name());//这个由QTreeView释放内存  
    }
    else
    {
        return m_pItem;
    }

    if(m_pItem)
    {
        m_pItem->setData(m_pRoom->jid(), ROLE_GROUPCHAT_JID);
        QVariant v;
        v.setValue(this);
        m_pItem->setData(v, ROLE_GROUPCHAT_OBJECT);
        m_pItem->setToolTip(m_pRoom->jid());
    }
    return m_pItem;
}

void CFrmGroupChat::slotJoined()
{
    if(m_pRoom)
        emit sigJoined(m_pRoom->jid(), this);
}

void CFrmGroupChat::slotLeft()
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotLeft");
    emit sigLeft(m_pRoom->jid(), this);
    this->deleteLater();
}

void CFrmGroupChat::slotNameChanged(const QString &name)
{
    if(m_pItem)
        m_pItem->setData(name, Qt::DisplayRole);
    ChangeTitle();
}

void CFrmGroupChat::slotError(const QXmppStanza::Error &error)
{
    LOG_MODEL_DEBUG("Group chat", "code:%d;type:%d;Condition:%d;error:%s", 
                    error.code(),
                    error.type(),
                    error.condition(),
                    qPrintable(error.text()));
    QString szMsg;
    switch(error.type())
    {
    case QXmppStanza::Error::Auth:
        szMsg = tr("Don't join room");
        break;
    default:
        szMsg = tr("Error code:") + QString::number(error.code())
                + tr("type:") + QString::number(error.type())
                + tr("condition:") + QString::number(error.condition())
                + tr("text:") + error.text();
    };
    QMessageBox::critical(this, tr("Error"), szMsg);
}

void CFrmGroupChat::slotAllowedActionsChanged(QXmppMucRoom::Actions actions) const
{
    LOG_MODEL_DEBUG("Group chat", "actions:%d", actions);
}

void CFrmGroupChat::slotConfigurationReceived(const QXmppDataForm &configuration)
{
    Q_UNUSED(configuration);
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotConfigurationReceived");
    //正常流程是先请求配置，然后在请求配置信号中处理配置  
    //并发送设置配置。但是join后直接设置配置在openfire中是可以的  
    //m_pRoom->setConfiguration(m_DataForm);
}

void CFrmGroupChat::slotSubjectChanged(const QString &subject)
{
    Q_UNUSED(subject);
    ChangeTitle();
}

void CFrmGroupChat::slotMessageReceived(const QXmppMessage &message)
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotMessageReceived:type:%d;state:%d;from:%s;to:%s;body:%s",
           message.type(),
           message.state(), //消息的状态 0:消息内容，其它值表示这个消息的状态  
           qPrintable(message.from()),
           qPrintable(message.to()),
           qPrintable(message.body())
          );

    if(QXmppUtils::jidToBareJid(message.from()) != QXmppUtils::jidToBareJid(m_pRoom->jid()))
    {
        LOG_MODEL_DEBUG("Group chat", "the room is %s, from %s received",
                        m_pRoom->jid().toStdString().c_str(),
                        message.from().toStdString().c_str());
        return;
    }

    if(message.body().isEmpty())
        return;

    QString nick;
    nick = QXmppUtils::jidToResource(message.from());
    AppendMessageToList(message.body(), nick);
}

void CFrmGroupChat::slotParticipantAdded(const QString &jid)
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotParticipantAdded:jid:%s", qPrintable(jid));
    QStandardItem* pItem = new QStandardItem(QXmppUtils::jidToResource(jid));
    m_pModel->appendRow(pItem);
}

void CFrmGroupChat::slotParticipantChanged(const QString &jid)
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotParticipantChanged:jid:%s", qPrintable(jid));
}

void CFrmGroupChat::slotParticipantRemoved(const QString &jid)
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChat::slotParticipantRemoved:jid:%s", qPrintable(jid));
    QList<QStandardItem*> items = m_pModel->findItems(QXmppUtils::jidToResource(jid));
    QStandardItem* item;
    foreach(item, items)
    {
        m_pModel->removeRow(item->row());
    }
}

void CFrmGroupChat::slotPermissionsReceived(const QList<QXmppMucItem> &permissions)
{
    QXmppMucItem item;
    foreach(item, permissions)
    {
        LOG_MODEL_DEBUG("Group chat", "jid:%s;nick:%s;actor:%s;affiliation:%s;role:%s",
                        item.jid().toStdString().c_str(),
                        item.nick().toStdString().c_str(),
                        item.actor().toStdString().c_str(),
                        QXmppMucItem::affiliationToString(item.affiliation()).toStdString().c_str(),
                        QXmppMucItem::roleToString(item.role()).toStdString().c_str());
    }
}

void CFrmGroupChat::on_pbSend_clicked()
{
    QString msg = ui->txtInput->toPlainText();
    if(msg.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("There is empty, please input."));
        return;
    }

    //AppendMessageToList(msg, m_pRoom->nickName());//服务器会转发，所以不需要  
    m_pRoom->sendMessage(msg);
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
    if(m_pRoom->nickName() != nick)
        msg += CGlobal::Instance()->GetRosterColor().name();
    else
        msg += CGlobal::Instance()->GetUserColor().name();
    msg += "'>[";
    msg += QTime::currentTime().toString() + "]" + nick + ":</font><br /><font color='";
    if(m_pRoom->nickName() != nick)
        msg += CGlobal::Instance()->GetRosterMessageColor().name();
    else
        msg += CGlobal::Instance()->GetUserMessageColor().name();
    msg += "'>";
    msg += recMsg.replace(QString("\n"), QString("<br />"));
    msg += "</font>";
    ui->txtView->append(msg);
    return 0;
}

int CFrmGroupChat::ChangeTitle()
{
    this->setWindowTitle(tr("Group chat [%1]:%2").arg(m_pRoom->name(), m_pRoom->subject()));
    return 0;
}
