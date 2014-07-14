#include "FrmMessage.h"
#include "ui_FrmMessage.h"
#include "../FrmUserList/Roster.h"
#include "qxmpp/QXmppMessage.h"
#include "../../MainWindow.h"
#include "../FrmVideo/FrmVideo.h"

CFrmMessage::CFrmMessage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmMessage)
{
    ui->setupUi(this);
    ui->txtInput->setFocus();//设置焦点
    m_pRoster = NULL;
    m_pMainWindow = NULL;

    //发送文件信号连接20140710
    {
        QAction* pAction = m_MoreMenu.addAction(tr("send file"));
        bool check = connect(pAction, SIGNAL(triggered()), SLOT(slotSendFileTriggered()));
        Q_ASSERT(check);
    }
    ui->tbMore->setMenu(&m_MoreMenu);
}

CFrmMessage::~CFrmMessage()
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::~CFrmMessage");
    delete ui;
}

//注意：只在对话框初始化后调用一次,必须最先调用一次
int CFrmMessage::SetRoster(CRoster* pRoster, MainWindow *pMainWindow)
{
    m_pRoster = pRoster;
    m_pMainWindow = pMainWindow;

    bool check = connect(m_pRoster, SIGNAL(sigChangedPresence(QXmppPresence::AvailableStatusType)),
                         SLOT(ChangedPresence(QXmppPresence::AvailableStatusType)));
    Q_ASSERT(check);

    return 0;
}

void CFrmMessage::ChangedPresence(QXmppPresence::AvailableStatusType status)
{
    ui->lbRosterName->setText(m_pRoster->Name()
                              + "["
                              + g_Global.GetStatusText(status)
                              + "]");
}

void CFrmMessage::slotSendFileTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "F:",
                                                    tr("All (*.*)"));
    if(fileName.isEmpty())
    {
        return;
    }
    QString jid = QString("%1/%2").arg(m_pRoster->Jid()).arg("QXmpp");//再定QXmpp,应该根据实际的resource
    m_pMainWindow->sendFile(jid,fileName);
}

void CFrmMessage::hideEvent(QHideEvent *)
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::hideEvent");
    m_pRoster->CleanNewMessageNumber();
}
void CFrmMessage::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Message", "CFrmMessage::closeEvent");
}

void CFrmMessage::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("Message", "CFrmMessage::showEvent");
    //TODO:重读数据
    if(m_pRoster)
    {
        ui->lbRosterName->setText(m_pRoster->Name()
                              + "["
                              + g_Global.GetStatusText(m_pRoster->GetStatus())
                              + "]");
    }
}

void CFrmMessage::on_pbBack_clicked()
{
    close();
}

int CFrmMessage::AppendMessageToList(const QString &szMessage, const QString &name, bool bRemote)
{
    QString recMsg = szMessage;
    QString msg("<font color='");
    if(bRemote)
        msg += g_Global.GetRosterColor().name();
     else
         msg += g_Global.GetUserColor().name();
    msg += "'>[";
    msg += QTime::currentTime().toString()  +  "]" + name +  ":</font><br /><font color='black'>";
    msg += recMsg.replace(QString("\n"), QString("<br />")) +  "</font><p />";
    ui->txtView->append(msg);
    return 0;
}

int CFrmMessage::AppendMessage(const QString &szMessage)
{
    AppendMessageToList(szMessage, m_pRoster->Name(), true);
    return 0;
}

void CFrmMessage::on_pbSend_clicked()
{
    //QString message=ui->txtInput->toHtml();
    //LOG_MODEL_DEBUG("FrmMessage", "message:%s", message.toStdString().c_str());
    
    AppendMessageToList(ui->txtInput->toPlainText());

    //发送
    QXmppMessage msg("", m_pRoster->BareJid(), ui->txtInput->toPlainText());
    m_pMainWindow->m_pClient->sendPacket(msg);

    ui->txtInput->clear();//清空输入框中的内容
}

void CFrmMessage::on_tbMore_clicked()
{
}

void CFrmMessage::on_pbVideo_clicked()
{
    CFrmVideo *pVideo = CFrmVideo::instance(m_pMainWindow->m_pClient);
    if(NULL == pVideo)
        return;
    
    QDesktopWidget *pDesk = QApplication::desktop();
    pVideo->move((pDesk->width() - pVideo->width()) / 2, (pDesk->height() - pVideo->height()) / 2);
    pVideo->show();
    pVideo->activateWindow();
    pVideo->Call(m_pRoster->BareJid() + "/QXmpp");
}
