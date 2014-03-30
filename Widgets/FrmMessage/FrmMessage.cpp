#include "FrmMessage.h"
#include "ui_FrmMessage.h"
#include "../FrmUserList/Roster.h"
#include "qxmpp/QXmppMessage.h"
#include "../../MainWindow.h"

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
    qDebug("CFrmMessage::~CFrmMessage");
    delete ui;
}

int CFrmMessage::SetRoster(CRoster* pRoster, MainWindow *pMainWindow)
{
    m_pRoster = pRoster;
    m_pMainWindow = pMainWindow;
    return 0;
}

void CFrmMessage::hideEvent(QHideEvent *)
{
    qDebug("CFrmMessage::hideEvent");
    m_pRoster->CleanNewMessageNumber();
}
void CFrmMessage::closeEvent(QCloseEvent *e)
{
    qDebug("CFrmMessage::closeEvent");
}

void CFrmMessage::showEvent(QShowEvent *)
{
    qDebug("CFrmMessage::showEvent");
    //TODO:重读数据
    ui->lbRosterName->setText(m_pRoster->Name());

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
    m_pRoster->m_Video.show();
    m_pRoster->m_Video.activateWindow();
    m_pRoster->m_Video.Call(m_pRoster->BareJid() + "/QXmpp");
}
