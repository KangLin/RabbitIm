#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include "qxmpp/QXmppRosterManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化子窗体
    m_pLogin = new CFrmLogin(this);
    this->setCentralWidget(m_pLogin);
    m_pUserList = NULL;
    m_pClient = new CXmppClient(this);
    if(m_pClient)
    {
        //初始化qxmpp log
        m_pClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);

        bool check;
        Q_UNUSED(check);

        check = connect(m_pClient, SIGNAL(disconnected()),
                        SLOT(clientDisconnected()));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(error(QXmppClient::Error)),
                        SLOT(clientError(QXmppClient::Error)));
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
}

void MainWindow::showEvent(QShowEvent *)
{
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    qDebug("MainWindow::closeEvent");
    QMessageBox msg(QMessageBox::Question,
                    tr("Close"),
                    tr("Is close the programe?"),
                    QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == msg.exec())
    {
        //退出程序
        e->accept();
        QCoreApplication::exit(0);
    }
    else
        e->ignore(); //忽略退出事件
}


void MainWindow::clientConnected()
{
    qDebug("MainWindow:: CONNECTED");
    //关闭登录对话框
    if(m_pLogin)
    {
        m_pLogin->close();
        delete m_pLogin;
        m_pLogin = NULL;
    }

    //显示好友列表
    if(NULL == m_pUserList)
        m_pUserList = new CFrmUserList(this);

    if(m_pUserList)
    {
        this->setCentralWidget(m_pUserList);
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
        m_pLogin->SetPrompt(tr("Network error"));

    }
    else if(QXmppClient::XmppStreamError == e)
    {
        m_pLogin->SetPrompt(tr("User name or password error"));

    }
    else if(QXmppClient::KeepAliveError == e)
        m_pLogin->SetPrompt(tr("Keep Alive error"));
    else
        m_pLogin->SetPrompt(tr("Login success"));

}

void MainWindow::clientIqReceived(const QXmppIq &iq)
{
    qDebug("MainWindow:: iq Received:%d", iq.type());
}
