#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "../../Global/Global.h"
#include "Version.h"
#include <QFile>
#include "../../MainWindow.h"

CDlgAbout::CDlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgAbout)
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

    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->geometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif

}

CDlgAbout::~CDlgAbout()
{
    LOG_MODEL_DEBUG("About", "CFrmAbout::~CFrmAbout");
    delete ui;
}

void CDlgAbout::showEvent(QShowEvent *)
{
}

void CDlgAbout::closeEvent(QCloseEvent *)
{
}

void CDlgAbout::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgAbout::on_btbButtons_clicked(QAbstractButton *button)
{
    Q_UNUSED(button);
    this->accept();
}
