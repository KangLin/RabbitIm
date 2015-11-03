#include <QDesktopServices>
#include <QMessageBox>
#include "DlgScanQRcode.h"
#include "ui_DlgScanQRcode.h"
#include "Tool.h"
#ifdef RABBITIM_USE_QZXING
    #include "qzxing.h"
#endif

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
    
#ifdef RABBITIM_USE_QZXING
    QZXing decoder;
    QString szMessage = decoder.decodeImage(img);
    if(szMessage.isEmpty())
    {
        LOG_MODEL_ERROR("CDlgScanQRcode", "Scan file:%s",
                        szFile.toStdString().c_str());
        return -2;
    }
#endif
    ui->lbText->setText(szMessage);
    return ProcessMessage(szMessage);
}

int CDlgScanQRcode::ProcessMessage(QString szMessage)
{
    QUrl url(szMessage);
    if("rabbitim" == url.scheme())
    {
        if("id" == url.host()) //增加好友  
        {
            QString szId;
            szId = szMessage.right(szMessage.size()
                                   - QString("rabbitim://id/").size());
            LOG_MODEL_DEBUG("CDlgScanQRcode", "ID:%s", szId.toStdString().c_str());
            if(szId.isEmpty())
                return -1;
            if(USER_INFO_LOCALE->GetInfo()->GetId() == szId)
            {
                LOG_MODEL_ERROR("CDlgScanQRcode", "Roster[%s] has exist.",
                                szId.toStdString().c_str());
                return -2;
            }
            QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szId);
            if(user.isNull())
            {
                GET_CLIENT->RosterAdd(szId);
                return 0;
            }
        }
    } else {
        if(url.scheme() == "http"
                || url.scheme() == "https"
                || url.scheme() == "ftp"
                || url.scheme() == "mailto")
        {
            QMessageBox msg(QMessageBox::Question, "",
                            tr("Do open ") + szMessage + "?",
                            QMessageBox::Ok | QMessageBox::No);
            if(QMessageBox::Ok == msg.exec())
                if(QDesktopServices::openUrl(url))
                    return 0;
        }
    }
    return -3;
}
