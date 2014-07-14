#include "FrmAbout.h"
#include "ui_FrmAbout.h"
#include "../../Global.h"
#include "Version.h"
#include <QFile>

CFrmAbout::CFrmAbout(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmAbout)
{
    ui->setupUi(this);
    QString szVersion(tr("Version:%1").arg(VERSION));
    ui->lblVersion->setText(szVersion);
    ui->lblAuthor->setText(tr("Author: KangLin Email、MSN:kl222@126.com"));
    QFile fileLicense( CGlobal::Instance()->GetDirApplication() + "License.txt");
    if(fileLicense.open(QFile::ReadOnly))
    {
        ui->txtLicense->append(fileLicense.readAll());
        fileLicense.close();
    }

    QFile fileReadMe(CGlobal::Instance()->GetDirApplication() + "README.md");
    if(fileReadMe.open(QFile::ReadOnly))
    {
        ui->txtReadMe->append(fileReadMe.readAll());
        fileReadMe.close();
    }
}

CFrmAbout::~CFrmAbout()
{
    LOG_MODEL_DEBUG("About", "CFrmAbout::~CFrmAbout");
    delete ui;
}

void CFrmAbout::closeEvent(QCloseEvent *)
{
    deleteLater();
}

void CFrmAbout::on_btbButtons_clicked(QAbstractButton *button)
{
    Q_UNUSED(button);
    this->close();
}
