#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化子窗体
    m_pLogin = new CFrmLogin(this);
    m_pUserList = NULL;
    m_pClient = new CXmppClient(this);
    if(m_pClient)
    {
        //初始化qxmpp log
        m_pClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);

        bool check;
        Q_UNUSED(check);

        check = connect(m_pClient, SIGNAL(connected()),
                        SLOT(clientConnected()));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(disconnected()),
                        SLOT(clientDisconnected()));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(error(QXmppClient::Error)),
                        SLOT(clientError(QXmppClient::Error)));
        Q_ASSERT(check);

        check = connect(&(m_pClient->rosterManager()), SIGNAL(rosterReceived()),
                        SLOT(rosterReceived()));
        Q_ASSERT(check);

        /// Then QXmppRoster::presenceChanged() is emitted whenever presence of someone
        /// in roster changes
        check = connect(&(m_pClient->rosterManager()), SIGNAL(presenceChanged(QString,QString)),
                        SLOT(presenceChanged(QString,QString)));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(messageReceived(QXmppMessage)),
                        SLOT(clientMessageReceived(QXmppMessage)));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(iqReceived(QXmppIq)),
                        SLOT(clientIqReceived(QXmppIq)));
        Q_ASSERT(check);
    }
}

MainWindow::~MainWindow()
{
    if(m_pLogin)
        delete m_pLogin;

    if(m_pClient)
        delete m_pClient;

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if(m_pLogin)
        m_pLogin->setFixedSize(this->geometry().width(),
                           this->geometry().height());

    if(m_pUserList)
        m_pUserList->setFixedSize(this->geometry().width(),
                                  this->geometry().height());
}

void MainWindow::showEvent(QShowEvent *)
{
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msg(QMessageBox::Question,
                    tr("Close"),
                    tr("Is close the programe?"),
                    QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == msg.exec())
    {
        //退出程序
        e->accept();
    }
    else
        e->ignore(); //忽略退出事件
}


void MainWindow::clientConnected()
{
    qDebug("MainWindow:: CONNECTED");
    if(m_pLogin)
    {
        m_pLogin->close();
        delete m_pLogin;
        m_pLogin = NULL;
    }

    if(NULL == m_pUserList)
        m_pUserList = new CFrmUserList(this);

    if(m_pUserList)
    {
        m_pUserList->setFixedSize(this->geometry().width(),
                                  this->geometry().height());
        m_pUserList->show();
    }
}

void MainWindow::clientDisconnected()
{
    qDebug("MainWindow:: DISCONNECTED");
}

void MainWindow::clientError(QXmppClient::Error e)
{
    qDebug("MainWindow:: Error:%d", e);
    if(!m_pLogin)
        return;

    if(QXmppClient::SocketError == e)
    {
        m_pLogin->SetPrompt(tr("Socket error"));

    }
    else if(QXmppClient::XmppStreamError == e)
    {
        m_pLogin->SetPrompt(tr("User name or password error"));

    }
    else
        m_pLogin->SetPrompt(tr("Login success"));

}

void MainWindow::rosterReceived()
{
    qDebug("MainWindow:: Roster received");
    foreach (const QString &bareJid, m_pClient->rosterManager().getRosterBareJids())
    {
        QString name = m_pClient->rosterManager().getRosterEntry(bareJid).name();
        if(name.isEmpty())
            name = "-";
        QSet<QString> groups = m_pClient->rosterManager().getRosterEntry(bareJid).groups();

        m_pUserList->InsertUser(bareJid, name, groups);
        qDebug("MainWindow:: Roster received: %s [%s]", qPrintable(bareJid), qPrintable(name));
    }
}

void MainWindow::presenceChanged(const QString& bareJid,
                                 const QString& resource)
{
    qDebug("MainWindow:: Presence changed %s/%s", qPrintable(bareJid), qPrintable(resource));
}

void MainWindow::clientMessageReceived(const QXmppMessage &message)
{
    qDebug("MainWindow:: message Received:%s", qPrintable(message.to()));
}

void MainWindow::clientIqReceived(const QXmppIq &iq)
{
    qDebug("MainWindow:: iq Received:%s", qPrintable(iq.to()));
}
