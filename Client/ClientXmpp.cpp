#include "ClientXmpp.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"
#include "qxmpp/QXmppConfiguration.h"
#include "qxmpp/QXmppIq.h"
#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppRegisterIq.h"
#include "qxmpp/QXmppTransferManager.h"
#include "UserInfo/UserInfoXmpp.h"
#include "Manager/ManageUserQXmpp.h"
#include "Global/Global.h"
#include <QImageWriter>
#include <QBuffer>
#include "MainWindow.h"
#include "FileTransfer/FileTransferQXmpp.h"
#include "Call/CallQXmpp.h"
#include <QHostAddress>

#ifdef WIN32
#undef GetMessage
#undef SendMessage
#endif

CClientXmpp::CClientXmpp(QObject *parent)
    : CClient(parent),
      m_User(NULL)
{
    //初始化qxmpp log
    m_Client.logger()->setLoggingType(QXmppLogger::StdoutLogging);

    m_Client.addExtension(&m_CallManager);
    m_Client.addExtension(&m_MucManager);
    //TODO:增加文件代理的查找,如果用发现服务查找， 
    //则可能引起大量查询包，导制服务器忙。目前解决方案就是直接设置代理  
    m_TransferManager.setProxy("proxy." + CGlobal::Instance()->GetXmppDomain());
    m_Client.addExtension(&m_TransferManager);

    InitConnect();
}

CClientXmpp::~CClientXmpp()
{
}

int CClientXmpp::InitConnect()
{
    bool check = false;
    m_Client.disconnect(this);

    check = connect(&m_Client, SIGNAL(error(QXmppClient::Error)),
                    SLOT(slotClientError(QXmppClient::Error)));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(connected()),
                    SLOT(slotClientConnected()));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(iqReceived(QXmppIq)),
                    SLOT(slotClientIqReceived(QXmppIq)));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(stateChanged(QXmppClient::State)),
                    SLOT(slotStateChanged(QXmppClient::State)));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(disconnected()),
                    SLOT(slotClientDisConnected()));
    Q_ASSERT(check);

    check = connect(&m_Client.rosterManager(), SIGNAL(rosterReceived()),
                    SLOT(slotRosterReceived()));
    Q_ASSERT(check);

    check = connect(&(m_Client.vCardManager()), SIGNAL(clientVCardReceived()),
                    SLOT(slotClientVCardReceived()));
    Q_ASSERT(check);

    check = connect(&m_Client.vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq&)),
                    SLOT(slotvCardReceived(const QXmppVCardIq&)));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(presenceReceived(const QXmppPresence)),
                    SLOT(slotPresenceReceived(QXmppPresence)));
    Q_ASSERT(check);

    check = connect(&m_Client.rosterManager(), SIGNAL(subscriptionReceived(const QString&)),
                    SLOT(slotSubscriptionReceived(const QString&)));
    Q_ASSERT(check);

    check = connect(&m_Client.rosterManager(), SIGNAL(itemAdded(QString)),
                    SLOT(slotItemAdded(QString)));
    Q_ASSERT(check);

    check = connect(&m_Client.rosterManager(), SIGNAL(itemChanged(QString)),
                    SLOT(slotItemChanged(QString)));
    Q_ASSERT(check);

    check = connect(&m_Client.rosterManager(), SIGNAL(itemRemoved(QString)),
                    SLOT(slotItemRemoved(QString)));
    Q_ASSERT(check);

    check = connect(&m_Client, SIGNAL(messageReceived(const QXmppMessage&)),
                    SLOT(slotMessageReceived(const QXmppMessage&)));
    Q_ASSERT(check);

    check = connect(&this->m_TransferManager, SIGNAL(fileReceived(QXmppTransferJob*)),
                    SLOT(slotFileReceived(QXmppTransferJob*)));
    Q_ASSERT(check);

    check = connect(&m_CallManager, SIGNAL(callReceived(QXmppCall*)),
                    SLOT(slotCallReceived(QXmppCall*)));
    Q_ASSERT(check);

    return 0;
}

int CClientXmpp::Register(const QString &szId, const QString &szName, const QString &szPassword, const QString &szEmail, const QString &szDescript)
{
    QXmppRegisterIq registerIq;
    registerIq.setType(QXmppIq::Set);
    registerIq.setUsername(szId);
    Q_UNUSED(szName);
    registerIq.setPassword(szPassword);
    registerIq.setEmail(szEmail);
    registerIq.setInstructions(szDescript);
    m_Client.sendPacket(registerIq);
    return 0;
}

int CClientXmpp::Login(const QString &szUserName, const QString &szPassword, CUserInfo::USER_INFO_STATUS status)
{
    QXmppConfiguration config;
    //TODO:设置为非sasl验证  
    config.setUseSASLAuthentication(false);
    if(szUserName.isNull())
        config.setUseNonSASLAuthentication(false);
    config.setHost(CGlobal::Instance()->GetXmppServer());
    config.setPort(CGlobal::Instance()->GetXmppServerPort());
    config.setDomain(CGlobal::Instance()->GetXmppDomain());
    if(!szUserName.isEmpty())
        config.setUser(szUserName);
    if(!szPassword.isEmpty())
        config.setPassword(szPassword);

    QXmppPresence presence;
    presence.setAvailableStatusType(StatusToPresence(status));
    m_Client.connectToServer(config, presence);
    return 0;
}

int CClientXmpp::Logout()
{
    SetLogin(false);
    QXmppPresence presence(QXmppPresence::Unavailable);
    m_Client.setClientPresence(presence);
    m_Client.disconnectFromServer();
    return 0;
}

int CClientXmpp::RequestUserInfoLocale()
{
    int nRet = 0;
    m_Client.vCardManager().requestClientVCard();
    return nRet;
}

int CClientXmpp::RequestUserInfoRoster(const QString& szId)
{
    m_Client.vCardManager().requestVCard(szId);
    return 0;
}

int CClientXmpp::setClientStatus(CUserInfo::USER_INFO_STATUS status)
{
    QXmppPresence presence;
    presence.setAvailableStatusType(StatusToPresence(status));
    m_Client.setClientPresence(presence);
    return 0;
}

int CClientXmpp::RosterAdd(const QString &szId, SUBSCRIBE_TYPE type, const QString &szName, const QSet<QString> &groups)
{
    QString id = szId;
    if(-1 == szId.indexOf("@"))
    {
        CUserInfoXmpp *pInfo = (CUserInfoXmpp*)USER_INFO_LOCALE->GetInfo().data();
        id = szId + "@" + pInfo->GetDomain();
    }
    else
        id = szId;

    //如果请求者是自己,则退出  
    if(USER_INFO_LOCALE->GetInfo()->GetId() == id)
    {
        LOG_MODEL_ERROR("CClientXmpp", "CClientXmpp::RosterAdd:Roster [%s] is exist", id.toStdString().c_str());
        return -1;
    }

    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp::RosterAdd:szId:%s", id.toStdString().c_str());
    switch(type)
    {
    case SUBSCRIBE_REQUEST:
        m_Client.rosterManager().addItem(id, szName, groups);
        m_Client.rosterManager().subscribe(id);
        break;
    case SUBSCRIBE_ACCEPT:
        if(m_User->GetUserInfoRoster(id).isNull())
        {
            m_Client.rosterManager().subscribe(id);
        }
        m_Client.rosterManager().addItem(id, szName, groups);
        m_Client.rosterManager().acceptSubscription(id);
        break;
    case SUBSCRIBE_REFUSE:
        m_Client.rosterManager().removeItem(id);
        m_Client.rosterManager().refuseSubscription(id);
        break;
    default:
        LOG_MODEL_ERROR("CClientXmpp", "Subscribe type is Invalid");
        break;
    }
    return 0;
}

int CClientXmpp::RosterRemove(const QString &szId)
{
    m_Client.rosterManager().unsubscribe(szId);
    return !m_Client.rosterManager().removeItem(szId);    
}

int CClientXmpp::RosterRename(const QString &szId, const QString &szName)
{
    return !m_Client.rosterManager().renameItem(szId, szName);
}

int CClientXmpp::SendMessage(const QString &szId, const QString &szMsg)
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    m_Client.sendMessage(szId, szMsg);
    roster->GetMessage()->AddMessage(szId, szMsg, true);
    emit sigMessageUpdate(szId);
    return 0;
}

int CClientXmpp::setlocaleUserInfo(QSharedPointer<CUserInfo> userInfo)
{
    QXmppVCardIq vCard;
    vCard.setFullName(userInfo->GetName());
    vCard.setNickName(userInfo->GetNick());
    vCard.setBirthday(userInfo->GetBirthday());
    vCard.setEmail(userInfo->GetEmail());
    vCard.setDescription(userInfo->GetDescription());
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    QImageWriter imgWriter(&buffer, "png");
    imgWriter.write(userInfo->GetPhoto());
    buffer.close();
    vCard.setPhoto(byteArray);
    m_Client.vCardManager().setClientVCard(vCard);
    RequestUserInfoLocale();
    return 0;
}

int CClientXmpp::SetUser(QSharedPointer<CManageUserQXmpp> user)
{
    m_User = user;
    return 0;
}

QSharedPointer<CFileTransfer> CClientXmpp::SendFile(const QString szId, const QString &szFile, const QString &szDescription)
{
    QSharedPointer<CUser> r = m_User->GetUserInfoRoster(szId);
    if(r.isNull())
    {
        LOG_MODEL_ERROR("CClientXmpp", "CClientXmpp::SendFile the roster is null");
        return QSharedPointer<CFileTransfer>();
    }

    CUserInfoXmpp* pInfo = (CUserInfoXmpp*)r->GetInfo().data();
    if(pInfo->GetResource().isEmpty())
    {
        LOG_MODEL_ERROR("CClientXmpp", "CClientXmpp::SendFile the roster resource is null");
        r->GetMessage()->AddMessage(szId, tr("The roster is offline, don't send the file."), true);
        emit sigMessageUpdate(szId);
        return QSharedPointer<CFileTransfer>();
    }

    QXmppTransferJob* pJob = m_TransferManager.sendFile(pInfo->GetJid(), szFile, szDescription);
    pJob->setLocalFileUrl(QUrl::fromLocalFile(szFile));
    QSharedPointer<CFileTransfer> file(new CFileTransferQXmpp(pJob));
    return file;
}

QSharedPointer<CCallObject> CClientXmpp::Call(const QString szId, bool bVideo)
{
    //检查被叫方是否在线  
    QSharedPointer<CUser> r = m_User->GetUserInfoRoster(szId);
    if(r.isNull())
    {
        LOG_MODEL_ERROR("CClientXmpp", "CClientXmpp::Call the roster is null");
        return QSharedPointer<CCallObject>();
    }
    CUserInfoXmpp* pInfo = (CUserInfoXmpp*)r->GetInfo().data();
    if(pInfo->GetResource().isEmpty())
    {
        LOG_MODEL_ERROR("CClientXmpp", "CClientXmpp::Call the roster resource is null");
        r->GetMessage()->AddMessage(szId, tr("The roster is offline, don't launch a call."), true);
        emit sigMessageUpdate(szId);
        return QSharedPointer<CCallObject>();
    }

    //设置ice服务器参数  
    m_CallManager.setStunServer(
                QHostAddress(CGlobal::Instance()->GetStunServer()),
                CGlobal::Instance()->GetStunServerPort()
                );
    m_CallManager.setTurnServer(
                QHostAddress(CGlobal::Instance()->GetTurnServer()),
                CGlobal::Instance()->GetTurnServerPort()
                );
    m_CallManager.setTurnUser(
                CGlobal::Instance()->GetTurnServerUser()
                );
    m_CallManager.setTurnPassword(
                CGlobal::Instance()->GetTurnServerPassword()
                );

    //返回QXmppCall的引用,这个会由QXmppCallManager管理.用户层不要释放此指针  
    QXmppCall* m_pCall = m_CallManager.call(pInfo->GetJid());
    if(NULL == m_pCall)
    {
        LOG_MODEL_ERROR("CClientXmpp",  "call %s fail", qPrintable(pInfo->GetJid()));
        return QSharedPointer<CCallObject>();
    }

    QSharedPointer<CCallObject> call(new CCallQXmpp(m_pCall, bVideo));
    return call;
}

QXmppPresence::AvailableStatusType CClientXmpp::StatusToPresence(CUserInfo::USER_INFO_STATUS status)
{
    QXmppPresence::AvailableStatusType s;
    switch (status) {
    case CUserInfo::Online:
        s = QXmppPresence::Online;
        break;
    case CUserInfo::Away:
        s = QXmppPresence::Away;
        break;
    case CUserInfo::Chat:
        s = QXmppPresence::Chat;
        break;
    case CUserInfo::DO_NOT_DISTURB:
        s = QXmppPresence::DND;
        break;
    case CUserInfo::Invisible:
        s = QXmppPresence::Invisible;
        break;
    case CUserInfo::OffLine:
        s = QXmppPresence::XA;
    default:
        break;
    }
    return s;
}

CUserInfo::USER_INFO_STATUS CClientXmpp::StatusFromPresence(QXmppPresence::AvailableStatusType status)
{
    CUserInfo::USER_INFO_STATUS s;
    switch(status)
    {
    case QXmppPresence::Online:
        s = CUserInfo::Online;
        break;
    case QXmppPresence::Away:
        s = CUserInfo::Away;
        break;
    case QXmppPresence::Chat:
        s = CUserInfo::Chat;
        break;
    case QXmppPresence::DND:
        s = CUserInfo::DO_NOT_DISTURB;
        break;
    case QXmppPresence::XA:
        s = CUserInfo::OffLine;
        break;
    case QXmppPresence::Invisible:
    default:
        s = CUserInfo::Invisible;
        break;
    }
    return s;
}

/**
 * @brief 客户端连接后进行初始化工作.注意:先初始化数据,再初始化界面  
 *
 */
void CClientXmpp::slotClientConnected()
{
    QString szId = m_Client.configuration().jidBare();

    int nRet = 0;
    if(!IsLogin())
    {
        //数据层在界面之前初始化  
        nRet = CGlobal::Instance()->GetManager()->Init(szId);
        if(nRet)
        {
            LOG_MODEL_ERROR("CClientXmpp", "Init GlobalUser fail");
            return;
        }

        //因为openfire当用户信息改变时，不会广播改变通知，所以当程序启动时要查询所有信息。这里会影响性能  
        //TODO:一种解决方案：只在查看用户信息时，才发送更新请求  
        if(USER_INFO_LOCALE.isNull())
        {
            //调用客户端操作，得到本地用户信息  
            GET_CLIENT->RequestUserInfoLocale();
        }

        SetLogin(true);//设置登录标志  
        emit sigClientConnected();
        emit sigLoadRosterFromStorage();
    }
}

/**
 * @brief 客户端断开连接时清理操作.注意:先清理界面,再清理数据  
 *
 */
void CClientXmpp::slotClientDisConnected()
{
    if(!IsLogin())
    {
        //注意:这个顺序不能变  
        emit sigClientDisconnected();
        CGlobal::Instance()->GetManager()->Clean();
    }
}

void CClientXmpp::slotClientError(QXmppClient::Error e)
{
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp:: Error:%d", e);

    ERROR_TYPE error;
    switch (e) {
    case QXmppClient::SocketError:
        error = NetworkError;
        break;
    case QXmppClient::XmppStreamError:
        error = LoginFail;
        break;
    case QXmppClient::KeepAliveError:
        error = KeepAliveError;
    default:
        error = NoError;
        break;
    }
    emit sigClientError(error);
}

void CClientXmpp::slotClientIqReceived(const QXmppIq &iq)
{
    ERROR_TYPE error = NoError;
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp:: iq Received:%d", iq.error().condition());
    if(iq.type() == QXmppIq::Result)
    {
        ;
    }
    else if(iq.type() == QXmppIq::Error)
    {
        LOG_MODEL_DEBUG("Register", "CFrmRegister::clientIqReceived:%d", iq.error().code());

        if(iq.error().condition() == QXmppIq::Error::Conflict)
        {
            error = Conflict;
        }
        else if(iq.error().condition() == QXmppIq::Error::InternalServerError)
        {
            error = InternalServerError;
        }
    }
    emit sigClientError(error);
    return;
}

void CClientXmpp::slotStateChanged(QXmppClient::State state)
{
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp::stateChanged, state:%d", state);

    //TODO:同一账户在不同地方登录。QXMPP没有提供错误状态  

    /*if(e.xmppStreamError().condition()
            == QXmppStanza::Error::Conflict)
    {
        QMessageBox msg(QMessageBox::QMessageBox::Critical,
                        tr("Error"),
                        tr("The user had logined in other place"),
                        QMessageBox::Ok);

        if(NULL == m_pLogin)
            m_pLogin = new CFrmLogin;

        if(m_pLogin)
        {
            this->setCentralWidget(m_pLogin);
        }
    }*/
}

void CClientXmpp::slotSubscriptionReceived(const QString &szId)
{
    emit sigRosterAddReceived(szId, SUBSCRIBE_REQUEST);
}

void CClientXmpp::slotRosterReceived()
{
    QStringList rosters = m_Client.rosterManager().getRosterBareJids();
    foreach(QString jid, rosters)
    {
        QSharedPointer<CUser> r = m_User->GetUserInfoRoster(jid);
        if(r.isNull())
        {
            LOG_MODEL_DEBUG("CClientXmpp", "slotRosterReceived:roster[%s] is not exist", jid.toStdString().c_str());
            r = m_User->AddUserInfoRoster(jid);
            QXmppRosterIq::Item item = m_Client.rosterManager().getRosterEntry(jid);
            m_User->UpdateUserInfoRoster(item);
            //因为openfire当用户信息改变时，不会广播改变通知，所以当程序启动时要查询所有信息。这里会影响性能  
            //TODO:一种解决方案：只在查看用户信息时，才发送更新请求  
            RequestUserInfoRoster(jid);
        }
    }
}

//得到本地用户形象信息  
void CClientXmpp::slotClientVCardReceived()
{
    LOG_MODEL_DEBUG("CClientXmpp", 
                    "CClientXmpp::slotClientVCardReceived:%s", 
                     m_Client.vCardManager().clientVCard().to().toStdString().c_str());

    m_User->UpdateUserInfoLocale(m_Client.vCardManager().clientVCard(), 
                               m_Client.vCardManager().clientVCard().to());
    //发信号  
    emit sigUpdateLocaleUserInfo();
}

//得到好友详细信息时触发  
void CClientXmpp::slotvCardReceived(const QXmppVCardIq& vCardIq)
{
    QString szJid = QXmppUtils::jidToBareJid(vCardIq.from());
    if(szJid.isEmpty())
        return;
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp::slotvCardReceived:%s", vCardIq.from().toStdString().c_str());
    QSharedPointer<CUser> r = m_User->GetUserInfoRoster(szJid);
    if(r.isNull())
    {
        QSharedPointer<CUser> user(new CUser);
        ((CUserInfoXmpp*)user->GetInfo().data())->UpdateUserInfo(vCardIq, vCardIq.from());
        r = user;
    }
    else//更新现在好友的信息  
        m_User->UpdateUserInfoRoster(vCardIq,
                               vCardIq.from());
    //发信号  
    emit sigUpdateRosterUserInfo(QXmppUtils::jidToBareJid(szJid), r);
}

void CClientXmpp::slotPresenceReceived(const QXmppPresence &presence)
{
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp::slotPresenceReceived jid:%s;status:%d;status text:%s",
           qPrintable(presence.from()),
           presence.availableStatusType(),
           qPrintable(presence.statusText())
           );

    //注意：这里的 barejid 是包含资源的  
    QString bareJid = QXmppUtils::jidToBareJid(presence.from());
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(bareJid);
    if(!roster.isNull())
    {
        if(presence.type() == QXmppPresence::Available)
        {
            CUserInfoXmpp* pInfo =  (CUserInfoXmpp*)roster->GetInfo().data();
            pInfo->UpdateStatus(StatusFromPresence(presence.availableStatusType()), presence.from());
        }
        else if(presence.type() == QXmppPresence::Unavailable)
        {
            CUserInfoXmpp* pInfo =  (CUserInfoXmpp*)roster->GetInfo().data();
            pInfo->UpdateStatus(CUserInfo::OffLine, presence.from());
        }
        //触发状态改变消息  
        emit sigChangedStatus(bareJid);
    }

    return;
}

void CClientXmpp::slotItemAdded(const QString &szId)
{
    QSharedPointer<CUser> r = m_User->GetUserInfoRoster(szId);
    if(r.isNull())
    {
        m_User->AddUserInfoRoster(szId);
        QXmppRosterIq::Item item = m_Client.rosterManager().getRosterEntry(szId);
        m_User->UpdateUserInfoRoster(item);
        RequestUserInfoRoster(szId);
    }
    else
        LOG_MODEL_DEBUG("CClientXmpp", "roster [%s] is exist.", szId.toStdString().c_str());
}

void CClientXmpp::slotItemChanged(const QString &szId)
{
    QSharedPointer<CUser> r = m_User->GetUserInfoRoster(szId);
    if(!r.isNull())
    {
        QXmppRosterIq::Item item = m_Client.rosterManager().getRosterEntry(szId);
        m_User->UpdateUserInfoRoster(item);
        //发信号  
        emit sigUpdateRosterUserInfo(QXmppUtils::jidToBareJid(szId), r);
    }
    else
        LOG_MODEL_DEBUG("CClientXmpp", "roster [%s] is not exist.", szId.toStdString().c_str());
}

void CClientXmpp::slotItemRemoved(const QString &szId)
{
    int nRet = 0;
    nRet = m_User->RemoveUserInfoRoster(szId);
    if(nRet)
    {
        LOG_MODEL_ERROR("CClientXmpp", "remove user info roster:%s", qPrintable(szId));
        return;
    }
    emit sigRemoveRosterUserInfo(szId);
}

void CClientXmpp::slotMessageReceived(const QXmppMessage &message)
{
    LOG_MODEL_DEBUG("CClientXmpp", "CClientXmpp::slotMessageReceived:type:%d;state:%d;from:%s;to:%s;body:%s",
                    message.type(),
                    message.state(), //消息的状态 0:消息内容，其它值表示这个消息的状态  
                    qPrintable(message.from()),
                    qPrintable(message.to()),
                    qPrintable(message.body())
                    );

    //是聊天消息  
    if(QXmppMessage::Chat == message.type() && QXmppMessage::None == message.state())
    {
        QString szId = QXmppUtils::jidToBareJid(message.from());
        QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
        roster->GetMessage()->AddMessage(szId, message.body());
        GET_MAINWINDOW->ShowTrayIconMessage(roster->GetInfo()->GetShowName() + ":", message.body());
        emit sigMessageUpdate(szId);
    }
    //是组消息,qxmpp在房间中响应此消息  
    /*if(QXmppMessage::GroupChat == message.type() && QXmppMessage::None == message.state())
    {
        QString szBareJid = QXmppUtils::jidToBareJid(message.from());
        QString szNick = QXmppUtils::jidToResource(message.from());
        //TODO:组消息  
    }*/
    //TODO:消息输入状态显示  
    
}

void CClientXmpp::slotFileReceived(QXmppTransferJob *job)
{
    QSharedPointer<CFileTransfer> file(new CFileTransferQXmpp(job));
    emit sigFileReceived(QXmppUtils::jidToBareJid(job->jid()), file);
}

void CClientXmpp::slotCallReceived(QXmppCall *pCall)
{
    QSharedPointer<CCallObject> call(new CCallQXmpp(pCall));
    emit sigCallReceived(call);
}
