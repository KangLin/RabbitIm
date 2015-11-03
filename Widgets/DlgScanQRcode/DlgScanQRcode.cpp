#include "DlgScanQRcode.h"
#include "ui_DlgScanQRcode.h"
#include "Tool.h"
#include "QRCode.h"

CDlgScanQRcode::CDlgScanQRcode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgScanQRcode)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    ui->lbQRCode->setText("");
    ui->lbText->setText("");
}

CDlgScanQRcode::~CDlgScanQRcode()
{
    delete ui;
}

void CDlgScanQRcode::on_pushBrowse_clicked()
{
    QString szFile, szFilter("*.PNG *.BMP *.JPG *.JPEG *.PBM *.PGM *.PPM *.XBM *.XPM");
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(szFile.isEmpty())
       return; 
    
    ui->leBrowse->setText(szFile);
    
    if(!ProcessQRFile(szFile))
        this->close();
}

int CDlgScanQRcode::ProcessQRFile(QString szFile)
{
    QImage img(szFile);
    if(img.isNull())
    {
        LOG_MODEL_ERROR("CDlgScanQRcode", "This isn't image file:%s",
                        szFile.toStdString().c_str());
        return -1;
    }
    ui->lbQRCode->setPixmap(QPixmap::fromImage(img));
    QString szText = CQRCode::ProcessQImage(img);
    ui->lbText->setText(szText);
    if(szText.isEmpty())
        return 0;
    return -2;
}
