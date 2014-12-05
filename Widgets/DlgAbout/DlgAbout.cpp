#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "Tool.h"
#include "Version.h"
#include <QFile>
#include "Global/Global.h"
#include <QDir>

CDlgAbout::CDlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgAbout)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    QString szVersion(tr("Version:%1").arg(VERSION));
    ui->lblVersion->setText(szVersion);
    ui->lblAuthor->setText(tr("Author: KangLin Email、MSN:kl222@126.com"));
    QFile readme( CGlobal::Instance()->GetDirApplication() + QDir::separator() + "README.md");
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
