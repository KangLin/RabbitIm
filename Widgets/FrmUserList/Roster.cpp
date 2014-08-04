#include "Roster.h"
#include "../../MainWindow.h"
#include "../../Global.h"
#include <QImageWriter>

CRoster::CRoster(QObject *parent) :
    QObject(parent)
{
    Init((MainWindow*)parent);
}

CRoster::CRoster(QXmppRosterIq::Item item, MainWindow *parent) : QObject(parent)
{
    Init(parent);
    m_RosterItem = item;
}

CRoster::~CRoster()
{
    LOG_MODEL_DEBUG("Roster", "CRoster::~CRoster");
    DeleteItems();
}

int CRoster::Init(MainWindow *parent)
{
    m_nNewMessageNumber = 0;
    m_pMainWindow = parent;

    m_Message.SetRoster(this, m_pMainWindow);

    return 0;
}

QString CRoster::ShowName()
{
    QString szText;
    switch(CGlobal::Instance()->GetRosterShowType())
    {
    case CGlobal::E_ROSTER_SHOW_JID:
        szText = this->BareJid();
        break;
    case CGlobal::E_ROSTER_SHOW_NAME:
        szText = this->Name();
        break;
    case CGlobal::E_ROSTER_SHOW_NICK:
    default:
        szText = Nick();
        break;
    }
    return szText;
}

QString CRoster::Name()
{
    QString n = m_RosterVCard.fullName();
    if(n.isEmpty())
        n = m_RosterItem.name();
    if(n.isEmpty())
        n = QXmppUtils::jidToUser(Jid());
    return n;
}

QString CRoster::BareJid()
{
    return QXmppUtils::jidToBareJid(Jid());
}

QString CRoster::Jid()
{
    QString s = m_szJid;
    if(s.isEmpty())
        s = m_RosterItem.bareJid();
    if(s.isEmpty()) //得到本地用户 JID  
        s = m_RosterVCard.to();
    return s;
}

QString CRoster::Domain()
{
    return QXmppUtils::jidToDomain(Jid());
}

QString CRoster::Resouce()
{
    return QXmppUtils::jidToResource(Jid());
}

//int CRoster::SetJid(QString jid)
//{
//    m_szJid = jid;
//    return 0;
//}

QString CRoster::Nick()
{
    return m_RosterVCard.nickName();
}

QDate CRoster::Birthday()
{
    return m_RosterVCard.birthday();
}

QString CRoster::Email()
{
    return m_RosterVCard.email();
}

QString CRoster::Description()
{
    return m_RosterVCard.description();
}

QImage CRoster::Photo()
{
    QByteArray photo = m_RosterVCard.photo();
    QBuffer buffer;
    buffer.setData(photo);
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    QImage image = imageReader.read();
    buffer.close();
    return image;
}

int CRoster::SetVCard(const QXmppVCardIq &vCard)
{
    m_RosterVCard = vCard;

    //保存头像到本地  
    QImageWriter imageWriter(CGlobal::Instance()->GetFileUserAvatar(this->BareJid()), "png");
    if(!imageWriter.write(Photo()))
        LOG_MODEL_ERROR("CRoster", "Save avater error, %s", imageWriter.errorString().toStdString().c_str());

    slotRefresh();
    return 0;
}

QSet<QString> CRoster::Groups()
{
    return m_RosterItem.groups();
}

QXmppPresence::AvailableStatusType CRoster::GetStatus()
{
    return m_Status;
}

QXmppRosterIq::Item::SubscriptionType CRoster::GetSubScriptionType()
{
    return m_RosterItem.subscriptionType();
}

QString CRoster::GetSubscriptionTypeStr(QXmppRosterIq::Item::SubscriptionType type) const
{
    switch(type)
    {
    case QXmppRosterIq::Item::NotSet:
        return "";
    case QXmppRosterIq::Item::None:
        return tr("[none]");
    case QXmppRosterIq::Item::Both:
        return "";
    case QXmppRosterIq::Item::From:
        return tr("[From]");
    case QXmppRosterIq::Item::To:
        return "";
    case QXmppRosterIq::Item::Remove:
        return tr("[remove]");
    default:
        {
            LOG_MODEL_WARNING("Roster", "QXmppRosterIq::Item::getTypeStr(): invalid type");
            return "";
        }
    }
}

QList<QStandardItem*> CRoster::GetItem()
{
    //呢称条目
    QStandardItem* pItem = new QStandardItem(ShowName() + GetSubscriptionTypeStr(GetSubScriptionType()));
    QVariant v;
    v.setValue(this);
    pItem->setData(v);
    pItem->setEditable(true);//允许双击编辑
    m_lstUserListItem.push_back(pItem);

    //消息条目
    QStandardItem* pMessageCountItem = new QStandardItem("");
    v.setValue(this);
    pMessageCountItem->setData(v);
    pMessageCountItem->setEditable(false);//禁止双击编辑
    m_lstMessageCountItem.push_back(pMessageCountItem);

    /* TODO:未读新消息数目树形控件中未显示 */
    QList<QStandardItem *> lstItems;
    lstItems.push_back(pItem);
    lstItems.push_back(pMessageCountItem);

    //更新条目显示内容
    UpdateItemDisplay();

    return lstItems;
}

int CRoster::DeleteItems()
{
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        if(p->parent())
        {
            p->parent()->removeRow(p->row()); //控件会自己释放 QStandardItem 内存
        }
    }
    m_lstUserListItem.clear();
    return 0;
}

int CRoster::UpdateItems(QXmppRosterIq::Item item)
{
    m_RosterItem = item;
    DeleteItems();
    return 0;
}

void CRoster::slotRefresh()
{
    ChangedPresence(m_szJid, m_Status);
}

int CRoster::UpdateItemDisplay()
{
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        //改变item背景颜色  
        p->setData(CGlobal::Instance()->GetRosterStatusColor(m_Status), Qt::BackgroundRole);
        
        QString szText;
        szText = ShowName()
                + "[" + CGlobal::Instance()->GetRosterStatusText(m_Status) + "]"
                +  GetSubscriptionTypeStr(GetSubScriptionType());
        p->setData(szText, Qt::DisplayRole); //改变item文本  

        //改变item图标  
        p->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(m_Status)), Qt::DecorationRole);
    }
    return 0;
}

int CRoster::ChangedPresence(const QString &jid, QXmppPresence::AvailableStatusType status)
{
    m_szJid = jid;
    m_Status = status;

    UpdateItemDisplay();

    emit sigChangedPresence(status);

    return 0;
}

int CRoster::ShowMessageDialog()
{
    int nRet = 0;

    QDesktopWidget *pDesk = QApplication::desktop();    
    m_Message.move((pDesk->width() - m_Message.width()) / 2, (pDesk->height() - m_Message.height()) / 2);
    m_Message.show();
    m_Message.activateWindow();//激活窗口，置顶

    return nRet;
}

int CRoster::AppendMessage(const QString &szMessage)
{
    if(m_Message.isHidden())
    {
        m_nNewMessageNumber++;
        //设置控件计数
        std::list<QStandardItem*>::iterator it;
        for(it = m_lstMessageCountItem.begin(); it != m_lstMessageCountItem.end(); it++)
        {
            QStandardItem* p = *it;
            p->setData(CGlobal::Instance()->GetRosterStatusColor(m_Status), Qt::BackgroundRole);
            QString szText = QString::number(m_nNewMessageNumber);
            p->setData(szText, Qt::DisplayRole);
        }

        //通知栏提示  
        m_pMainWindow->ShowTrayIconMessage(this->ShowName(), szMessage);
    }
    else
    {
        CleanNewMessageNumber();
    }
    return m_Message.AppendMessage(szMessage);
}

int CRoster::CleanNewMessageNumber()
{
    m_nNewMessageNumber = 0;
    //清除控件计数
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstMessageCountItem.begin(); it != m_lstMessageCountItem.end(); it++)
    {
        QStandardItem* p = *it;
        if(p)
        {
            p->setData(CGlobal::Instance()->GetRosterStatusColor(m_Status), Qt::BackgroundRole);
            p->setData("", Qt::DisplayRole);
        }
    }
    return 0;
}
