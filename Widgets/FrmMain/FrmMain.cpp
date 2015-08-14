#include "FrmMain.h"
#include "ui_FrmMain.h"
#include "../../Global/Global.h"
#include "MainWindow.h"
#include <QSettings>

CFrmMain::CFrmMain(QWidget *parent) :
    QFrame(parent),
    m_UserList(parent),
    m_GroupChatList(parent),
#ifdef RABBITIM_WEBKIT
    m_weather(new QWebView),
#endif
    ui(new Ui::CFrmMain)
{
    ui->setupUi(this);
    ui->tabWidget->clear();

    ui->lbAvatar->setScaledContents(true);
    ui->lbName->clear();
#ifdef RABBITIM_WEBKIT
    m_weather->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_weather->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_weather->setUrl(QUrl("http://p1234.vicp.net/weather/weather_small.html"));
    m_weather->setMaximumHeight(46);
    ui->HeadLayout->addWidget(m_weather.data());
    connect(m_weather.data(), SIGNAL(linkClicked(QUrl)),
            SLOT(slotWebViewLinkClicked(QUrl)));
#endif

    bool check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateLocaleUserInfo()),
                    SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);

    check = connect(ui->lbAvatar, SIGNAL(clicked()),
                    GET_MAINWINDOW, SLOT(slotEditInformation()));
    Q_ASSERT(check);
    check = connect(ui->lbName, SIGNAL(clicked()),
                    GET_MAINWINDOW, SLOT(slotEditInformation()));
    Q_ASSERT(check);

    ui->tabWidget->addTab(&m_UserList, QIcon(":/icon/User"), tr("Rosters"));
    ui->tabWidget->addTab(&m_GroupChatList, QIcon(":/icon/Users"), tr("Group Chat"));
    ui->tabWidget->addTab(&m_MessageList, QIcon(":/icon/Message"), tr("Recent messages"));
    ui->tabWidget->addTab(&m_Lbs, QIcon(":/png/motion"), tr("Motion"));

    if(USER_INFO_LOCALE.isNull() || USER_INFO_LOCALE->GetInfo().isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "locale user is null");
        return;
    }
    QSettings conf(CGlobal::Instance()->GetUserConfigureFile(USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
    int nIndex = conf.value("Widgets/Main", 0).toInt();
    ui->tabWidget->setCurrentIndex(nIndex);
}

CFrmMain::~CFrmMain()
{
    GET_CLIENT.data()->disconnect(this);
    
    if(!USER_INFO_LOCALE.isNull())
    {
        QSettings conf(CGlobal::Instance()->GetUserConfigureFile(USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
        conf.setValue("Widgets/Main", ui->tabWidget->currentIndex());
    }

    delete ui;
    ui = NULL;
}

void CFrmMain::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    /*LOG_MODEL_DEBUG("CWdgTableMain", "CWdgTableMain::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());*/

    //ui->tabWidget->resize(geometry().size().width(), geometry().size().height() - ui->lbAvatar->frameGeometry().height());
    //ui->tabWidget->currentWidget()->resize(ui->tabWidget->geometry().size());
}

void CFrmMain::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_UserList), tr("Rosters"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_MessageList), tr("Recent messages"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_GroupChatList), tr("Group Chat"));
        break;
    }
}

void CFrmMain::showEvent(QShowEvent *)
{
    slotUpdateLocaleUserInfo();
}

void CFrmMain::slotUpdateLocaleUserInfo()
{
    QSharedPointer<CUser> user = USER_INFO_LOCALE;
    if(user.isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "User is null");
        return;
    }
    QSharedPointer<CUserInfo> info = user->GetInfo();
    if(info.isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "User is null");
        return;
    }

    ui->lbName->setText(info->GetShowName());
    QPixmap pmp;
    MainWindow::ComposeAvatarStatus(info, pmp);
    ui->lbAvatar->clear();
    ui->lbAvatar->setPixmap(pmp);
}

#ifdef RABBITIM_WEBKIT
void CFrmMain::slotWebViewLinkClicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}
#endif
