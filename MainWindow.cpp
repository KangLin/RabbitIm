#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include "qxmpp/QXmppRosterManager.h"
#include "Widgets/FrmAbout/FrmAbout.h"
#include <QMessageBox>
#include "Widgets/FrmUserList/FrmUserList.h"
#include "Widgets/DlgLogin/FrmLogin.h"
#include "Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_pAppTranslator = NULL;
    m_pQtTranslator = NULL;

    ui->setupUi(this);

    bool check;
    Q_UNUSED(check);
    check = connect(ui->actionAbout_A, SIGNAL(triggered()),
            SLOT(About()));
    Q_ASSERT(check);

    //初始化子窗体
    m_pLogin = new CFrmLogin(this);
    this->setCentralWidget(m_pLogin);
    m_pUserList = NULL;
    m_pClient = new CXmppClient(this);
    if(m_pClient)
    {
        //初始化qxmpp log
        m_pClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);

        check = connect(m_pClient, SIGNAL(disconnected()),
                        SLOT(clientDisconnected()));
        Q_ASSERT(check);

        check = connect(m_pClient, SIGNAL(error(QXmppClient::Error)),
                        SLOT(clientError(QXmppClient::Error)));
        Q_ASSERT(check);

        /*check = connect(m_pClient, SIGNAL(iqReceived(QXmppIq)),
                        SLOT(clientIqReceived(QXmppIq)));
        Q_ASSERT(check);//*/

        check = connect(m_pClient, SIGNAL(stateChanged(QXmppClient::State)),
                        SLOT(stateChanged(QXmppClient::State)));
        Q_ASSERT(check);

        CFrmVideo::instance(m_pClient);
    }
}

MainWindow::~MainWindow()
{
    if(m_pLogin)
        delete m_pLogin;

    if(m_pClient)
        delete m_pClient;

    if(m_pUserList)
    {
        m_pUserList->DeleteFromMainMenu(ui->menuOperator_O);
        delete m_pUserList;
    }

    delete ui;

    if(m_pAppTranslator)
        delete m_pAppTranslator;
    if(m_pQtTranslator)
        delete m_pQtTranslator;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
}

void MainWindow::showEvent(QShowEvent *)
{
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::closeEvent");
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
    //关闭登录对话框  
    if(m_pLogin)
    {
        m_pLogin->SaveConf();
        m_pLogin->close();
        delete m_pLogin;
        m_pLogin = NULL;
    }

    //显示好友列表  
    if(NULL == m_pUserList)
    {
        m_pUserList = new CFrmUserList(this);
        if(m_pUserList)
        {    //注册菜单  
            m_pUserList->AddToMainMenu(ui->menuOperator_O, ui->actionExit_O);
        }
    }

    if(m_pUserList)
    {
        //把UserList设置到主窗口中心  
        this->setCentralWidget(m_pUserList);
    }
}

void MainWindow::clientDisconnected()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: DISCONNECTED");
}

void MainWindow::clientError(QXmppClient::Error e)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: Error:%d", e);
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
    LOG_MODEL_DEBUG("MainWindow", "MainWindow:: iq Received:%d", iq.error().condition());
}

void MainWindow::stateChanged(QXmppClient::State state)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::stateChanged, state:%d", state);

    //TODO:同一账户在不同地方登录。QXMPP没有提供错误状态 

    /*if(e.xmppStreamError().condition()
            == QXmppStanza::Error::Conflict)
    {
        QMessageBox msg(QMessageBox::QMessageBox::Critical,
                        tr("Error"),
                        tr("The user had logined in other place"),
                        QMessageBox::Ok);
        if(m_pUserList)
        {
            m_pUserList->close();
            delete m_pUserList;
            m_pUserList = NULL;
        }

        if(NULL == m_pLogin)
            m_pLogin = new CFrmLogin;

        if(m_pLogin)
        {
            this->setCentralWidget(m_pLogin);
        }
    }*/
}

void MainWindow::About()
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::About");
    CFrmAbout* pAbout = new CFrmAbout;//CFrmAbout 会在关闭时自动释放内存
    if(pAbout)
    {
        pAbout->show();
        pAbout->activateWindow();
    }
}

void MainWindow::on_actionChange_Style_Sheet_S_triggered()
{
    //*从资源中加载应用程序样式 
    QString szFile = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "*.qss");
    if(szFile.isEmpty())
        return;

    QFile file(szFile);//从资源文件中加载 
    if(file.open(QFile::ReadOnly))
    {
        QString stylesheet= file.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
        QSettings conf(g_Global.GetApplicationConfigureFile(), QSettings::IniFormat);
        conf.setValue("UI/StyleSheet", szFile);
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d", 
                        szFile.toStdString().c_str(), file.error());
    }//*/
}

//TODO:语言动态切换不对 
void MainWindow::on_actionEnglish_E_triggered()
{
    return;
    
    qApp->removeTranslator(m_pQtTranslator);
    qApp->removeTranslator(m_pAppTranslator);
}

//TODO:语言动态切换不对 
void MainWindow::on_actionChinese_C_triggered()
{
    return;
    
    qApp->removeTranslator(m_pQtTranslator);
    qApp->removeTranslator(m_pAppTranslator);

    if(m_pQtTranslator)
        delete m_pQtTranslator;
    m_pQtTranslator = new QTranslator;
    
    if(m_pQtTranslator)
        delete m_pQtTranslator;
    m_pQtTranslator = new QTranslator;
    
    //本地化QT资源
    QString szLocale = "zh_CN";
#ifdef DEBUG
    m_pQtTranslator->load("qt_" + szLocale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    LOG_MODEL_DEBUG("main", 
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString().c_str());
#else
    m_pQtTranslator->load("qt_" + szLocale,
                      QCoreApplication::applicationDirPath());
#endif
    qApp->installTranslator(m_pQtTranslator);

    //本地化程序资源 
    //把翻译文件放在了应用程序目录下,这样可以结约内存,适用于很多语言版本 
    //myappTranslator.load("app_" + locale, a.applicationDirPath());
    //把翻译文件放在了程序资源中 
    m_pAppTranslator->load(":/translations/" + szLocale);
    qApp->installTranslator(m_pAppTranslator);
}
