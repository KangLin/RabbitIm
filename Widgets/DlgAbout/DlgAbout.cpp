#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "common/Tool.h"
#include "common/QRCode.h"
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
 
    QString szVersion(tr("Version:%1.%2.%3.%4").arg(QString::number(REVISION_VERSION_NUMBER),
                                                 QString::number(MINOR_VERSION_NUMBER),
                                                 QString::number(REVISION_VERSION_NUMBER),
                                                 BUILD_VERSION
                                                    ));
    ui->lbVersion->setText(szVersion);
    ui->lbDate->setText(tr("Build date:%1 %2").arg(__DATE__, __TIME__));
    ui->lbAuthor->setText(tr("Author: KangLin\nEmail or MSN:kl222@126.com"));
    ui->lbHome->setOpenExternalLinks(true);
    QString szHomePage = "https://github.com/KangLin/rabbitim";
    ui->lbHome->setText(tr("Home page:") + "<a href=\"" + szHomePage + "\">" + szHomePage + "</a>");
    ui->lbCopyright->setText(tr(" Copyright (C) 2014 - %1 KangLin Studio").arg(QString::number(QDate::currentDate().year())));
    ui->lbQrencode->setText("");
    m_Image = CQRCode::QRcodeEncodeString(szHomePage, QImage(":/icon/AppIcon"));
    if(m_Image.isNull()) {
        ui->lbQrCodePrompt->setVisible(false);
        ui->lbQrencode->setVisible(false);
        ui->pbSave->setVisible(false);
    } else {
        ui->lbQrencode->setPixmap(QPixmap::fromImage(m_Image));
    }

    QString szFile;
#ifdef MOBILE
    szFile = ":/file/ChangeLog";
    AppendFile(ui->txtChange, szFile);
    szFile = ":/file/License";
    AppendFile(ui->txtLicense, szFile);
    szFile = ":/file/Authors";
    AppendFile(ui->txtThinks, szFile);
#else
    szFile = CGlobalDir::Instance()->GetDirApplication() + QDir::separator() + "ChangeLog.md";
    AppendFile(ui->txtChange, szFile);
    szFile = CGlobalDir::Instance()->GetDirApplication() + QDir::separator() + "License.md";
    AppendFile(ui->txtLicense, szFile);
    szFile = CGlobalDir::Instance()->GetDirApplication() + QDir::separator() + "Authors.txt";
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
        //把光标移动文档开始处  
        QTextCursor cursor =   pEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        pEdit->setTextCursor(cursor);
        readme.close();
    }
    return 0;
}

void CDlgAbout::on_pbSave_clicked()
{
    QString szFile, szFilter("*.PNG *.BMP *.JPG *.JPEG *.PBM *.PGM *.PPM *.XBM *.XPM");
    szFile = CTool::FileDialog(this, "QrRabbitim.PNG",
                               szFilter, tr("Save as"), QFileDialog::AcceptSave);
    if(szFile.isEmpty())
       return; 
    if(m_Image.isNull())
        return;
    if(!m_Image.save(szFile))
        LOG_MODEL_ERROR("CDlgAbout", "Save qrencode fail:%s",
                        szFile.toStdString().c_str());
}
