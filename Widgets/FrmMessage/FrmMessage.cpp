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
    m_pRoster = NULL;
    m_pMainWindow = NULL;

    m_pModel = new QStandardItemModel();
    ui->listView->setModel(m_pModel);
    //禁止编辑条目
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAutoScroll(true);
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
    QStandardItem* pRosterName = new QStandardItem(
                "[" + QTime::currentTime().toString() + "]"
                + name + ":");
    m_pModel->appendRow(pRosterName);
    if(bRemote)
        pRosterName->setData(g_Global.GetRosterColor(), Qt::ForegroundRole);
    else
        pRosterName->setData(g_Global.GetUserColor(), Qt::ForegroundRole);

    QStandardItem* pMessage = new QStandardItem(szMessage);
    m_pModel->appendRow(pMessage);

    //列表自动定位到最后一行
    QModelIndex index = m_pModel->index(m_pModel->rowCount() - 1, 0);
    ui->listView->setCurrentIndex(index);
    return 0;
}

int CFrmMessage::AppendMessage(const QString &szMessage)
{
    AppendMessageToList(szMessage, m_pRoster->Name(), true);
    return 0;
}

void CFrmMessage::on_pbSend_clicked()
{
    AppendMessageToList(ui->textEdit->toPlainText());

    //发送
    QXmppMessage msg("", m_pRoster->BareJid(), ui->textEdit->toPlainText());
    m_pMainWindow->m_pClient->sendPacket(msg);

    ui->textEdit->clear();//清空输入框中的内容
}

void CFrmMessage::on_tbMore_clicked()
{
}

void CFrmMessage::on_pbVideo_clicked()
{
    CFrmVideo *pVideo = CFrmVideo::instance(m_pMainWindow->m_pClient);
    if(NULL == pVideo)
        return;
    pVideo->show();
    pVideo->activateWindow();
    pVideo->Call(m_pRoster->BareJid() + "/QXmpp");
}
