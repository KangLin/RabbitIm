#include "FrmApp.h"
#include "ui_FrmApp.h"
#include <QSettings>
#include "Global/Global.h"

CFrmApp::CFrmApp(QWidget *parent) :
    QWidget(parent),
    m_AppList(parent),
    m_AppFavorites(parent),
    ui(new Ui::CFrmApp)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    ui->tabWidget->addTab(&m_AppList, QIcon(":/icon/Application"),
                          tr("Application"));
    ui->tabWidget->addTab(&m_AppFavorites, QIcon(":/icon/Favoris"),
                          tr("Favorites"));

    if(USER_INFO_LOCALE.isNull() || USER_INFO_LOCALE->GetInfo().isNull())
    {
        qCritical("Locale user is null");
        return;
    }
    QSettings conf(CGlobalDir::Instance()->GetUserConfigureFile(
              USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
    int nIndex = conf.value("Widgets/AppMain", 0).toInt();
    ui->tabWidget->setCurrentIndex(nIndex);
}

CFrmApp::~CFrmApp()
{
    if(!USER_INFO_LOCALE.isNull())
    {
        QSettings conf(CGlobalDir::Instance()->GetUserConfigureFile(
                  USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
        conf.setValue("Widgets/AppMain", ui->tabWidget->currentIndex());
    }
    delete ui;
}

void CFrmApp::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);

        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_AppList),
                                  tr("Application"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_AppFavorites),
                                  tr("Favorites"));
        break;
    }
}
