#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "Tool.h"
#include "Version.h"
#include <QFile>
#include "Global/Global.h"
#include <QDir>
#include <QDateTime>

CDlgAbout::CDlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgAbout)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
 
    QString szVersion(tr("Version:%1.%2.%3").arg(QString::number(REVISION_VERSION_NUMBER),
                                                 QString::number(MINOR_VERSION_NUMBER),
                                                 QString::number(REVISION_VERSION_NUMBER)
                                                    ));
    ui->lblVersion->setText(szVersion);
    ui->lbbuildVersion->setText(tr("Build version:%1").arg(BUILD_VERSION));
    ui->lbDate->setText(tr("Build date:%1 %2").arg(__DATE__, __TIME__));
    ui->lblAuthor->setText(tr("Author: KangLin\nEmail or MSN:kl222@126.com"));
    QFile readme(CGlobal::Instance()->GetDirApplication() + QDir::separator() + "README.md");
    if(readme.open(QFile::ReadOnly))
    {
        ui->txtReadMe->append(readme.readAll());
        readme.close();
    }
}

CDlgAbout::~CDlgAbout()
{
    delete ui;
}
