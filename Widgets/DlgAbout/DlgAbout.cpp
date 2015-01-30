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

    QString szFile;
#ifdef MOBILE
    szFile = ":/file/ChangeLog";
    AppendFile(ui->txtChange, szFile);
    szFile = ":/file/License";
    AppendFile(ui->txtLicense, szFile);
    szFile = ":/file/Authors";
    AppendFile(ui->txtThinks, szFile);
#else
    szFile = CGlobal::Instance()->GetDirApplication() + QDir::separator() + "ChangeLog.txt";
    AppendFile(ui->txtChange, szFile);
    szFile = CGlobal::Instance()->GetDirApplication() + QDir::separator() + "License.html";
    AppendFile(ui->txtLicense, szFile);
    szFile = CGlobal::Instance()->GetDirApplication() + QDir::separator() + "Authors.txt";
    AppendFile(ui->txtThinks, szFile);
#endif
}

CDlgAbout::~CDlgAbout()
{
    delete ui;
}

int CDlgAbout::AppendFile(QTextEdit* pEdit, const QString &szFile)
{
    QFile readme(szFile);
    if(readme.open(QFile::ReadOnly))
    {
        pEdit->append(readme.readAll());
        readme.close();
    }
    return 0;
}
