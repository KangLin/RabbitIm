#include "Roster.h"
#include "../../MainWindow.h"
#include "../../Global.h"
#include <QImageWriter>

CRoster::CRoster(QObject *parent) :
    QObject(parent)
{
    Init();
}

CRoster::CRoster(QXmppRosterIq::Item item, QObject *parent) : QObject(parent)
{
    Init();
    UpdateItemInfo(item);
}

CRoster::~CRoster()
{
    LOG_MODEL_DEBUG("Roster", "CRoster::~CRoster");
    DeleteItems();
}

int CRoster::Init()
{
    m_nNewMessageNumber = 0;

    m_Message.SetRoster(this);

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
    QString n = m_szName;
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
    return m_szJid;
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
    return m_szNick;
}

QDate CRoster::Birthday()
{
    return m_Birthday;
}

QString CRoster::Email()
{
    return m_szEmail;
}

QString CRoster::Description()
{
    return m_szDescription;
}

QImage CRoster::Photo()
{
    if(m_imgPhoto.isNull())
        return QImage(":/icon/AppIcon");

    return m_imgPhoto;
}

int CRoster::SetVCard(const QXmppVCardIq &vCard, QString jid)
{
    if(!vCard.fullName().isEmpty())
        m_szName = vCard.fullName();
    m_szNick = vCard.nickName();
    m_Birthday = vCard.birthday();
    m_szEmail = vCard.email();
    m_szDescription = vCard.description();
    //保存头像  
    QByteArray photo = vCard.photo();
    QBuffer buffer;
    buffer.setData(photo);
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    m_imgPhoto = imageReader.read();
    buffer.close();

    if(!jid.isEmpty())
        m_szJid = jid;
    //保存头像到本地  
    QImageWriter imageWriter(CGlobal::Instance()->GetFileUserAvatar(BareJid()), "png");
    if(!imageWriter.write(Photo()))
        LOG_MODEL_ERROR("CRoster", "Save avater error, %s", imageWriter.errorString().toStdString().c_str());

    slotRefresh();
    return 0;
}

QSet<QString> CRoster::Groups()
{
    return m_Groups;
}

QXmppPresence::AvailableStatusType CRoster::GetStatus()
{
    return m_Status;
}

QXmppRosterIq::Item::SubscriptionType CRoster::GetSubScriptionType()
{
    return m_subscriptionType;
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

    /* 未读新消息数目树形控件中显示 */
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

int CRoster::UpdateItems(QXmppRosterIq::Item &item)
{
    UpdateItemInfo(item);
    DeleteItems();
    return 0;
}

int CRoster::UpdateItemInfo(QXmppRosterIq::Item &item)
{
    m_szJid = item.bareJid();
    if(!item.name().isEmpty())
        m_szName = item.name();
    m_Groups = item.groups();
    m_subscriptionType = item.subscriptionType();
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
            //p->setData(CGlobal::Instance()->GetRosterStatusColor(m_Status), Qt::BackgroundRole);
            p->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
            QString szText = QString::number(m_nNewMessageNumber);
            p->setData(szText, Qt::DisplayRole);
        }

        //通知栏提示  
        CGlobal::Instance()->GetMainWindow()->ShowTrayIconMessage(this->ShowName(), szMessage);
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
            p->setData("", Qt::DisplayRole);
        }
    }
    return 0;
}

QDataStream &operator <<(QDataStream &output, const CRoster &roster)
{
    output << roster.m_szJid
           << roster.m_szName
           << roster.m_szNick
           << roster.m_szEmail
           << roster.m_szDescription
           << roster.m_Birthday
           << roster.m_Groups.size();
    QString szGroup;
    foreach(szGroup, roster.m_Groups)
    {
        output << szGroup;
    }

    return output;
}

QDataStream &operator >>(QDataStream &input, CRoster &roster)
{
    input >> roster.m_szJid 
            >> roster.m_szName
            >> roster.m_szName 
            >> roster.m_szEmail
            >> roster.m_szDescription 
            >> roster.m_Birthday;
    int nGroupSize;
    input >> nGroupSize;
    while(nGroupSize--)
    {
        QString szGroup;
        input >> szGroup;
        roster.m_Groups << szGroup;
    }

    QImageReader imgReader(CGlobal::Instance()->GetFileUserAvatar(roster.BareJid()), "png");
    if(!imgReader.read(&roster.m_imgPhoto))
        LOG_MODEL_ERROR("CRoster", "read avater error, %s", imgReader.errorString().toStdString().c_str());

    return input;
}
