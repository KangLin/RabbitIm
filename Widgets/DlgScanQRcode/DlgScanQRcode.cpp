#include "DlgScanQRcode.h"
#include "ui_DlgScanQRcode.h"
#include "Global/Global.h"
#include "QRCode.h"
#include <QPainter>
#include <QDir>
#include <QStandardPaths>

CDlgScanQRcode::CDlgScanQRcode(QWidget *parent) :
    QDialog(parent),
    m_Play(this),
    m_pCamera(NULL),
    ui(new Ui::CDlgScanQRcode)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
#ifdef MOBILE
    ui->Cancel->setText(tr("Back"));
#endif
    ui->glayPlay->addWidget(&m_Play);
    ui->lbText->setText("");
    ui->lbQRCode->setText("");
    ui->lbLogon->setText("");

    bool check = connect(&m_Timer, SIGNAL(timeout()), SLOT(OnTimeOut()));
    Q_ASSERT(check);

    m_pCamera = CCameraFactory::Instance()->GetCamera(0);
    if(m_pCamera)
    {
        m_pCamera->Open(this);
        Start();
    } else {
        ui->lbText->setText(tr("The camera does not exist."));
    }
}

CDlgScanQRcode::~CDlgScanQRcode()
{
    LOG_MODEL_DEBUG("CDlgScanQRcode", "CDlgScanQRcode::~CDlgScanQRcode");
    if(m_pCamera)
    {
        Stop();
        m_pCamera->Close();
        m_pCamera = NULL;
    }
    delete ui;
}

void CDlgScanQRcode::on_pushBrowse_clicked()
{
    Stop();

    QString szFile, szFilter(tr("Image Files (*.PNG *.BMP *.JPG *.JPEG *.PBM *.PGM *.PPM *.XBM *.XPM);;All Files (*.*)"));
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(!szFile.isEmpty())
    {
        /*QString szText;
        int nRet = CQRCode::ProcessQRFile(szFile, szText);
        ui->lbText->setText(szText);
        if(nRet)
            this->accept();//*/
        if(ProcessQRFile(szFile) == 0)
        {
            this->accept();
            return;
        }
    }
    Start();
}

void CDlgScanQRcode::on_Cancel_clicked()
{
    this->reject();
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

    QString szText;
    int nRet = CQRCode::ProcessQImage(img, szText);
    ui->lbText->setText(szText);
    return nRet;
}

int CDlgScanQRcode::OnFrame(const std::shared_ptr<CVideoFrame> frame)
{
    QString szText;
    m_Play.slotDisplayRemoteVideo(frame);
    return 0;
}

int CDlgScanQRcode::OnCapture(const std::string szFile)
{
    Stop();
    int nRet = ProcessQRFile(QString(szFile.c_str()));
    if(nRet <= 0)
    {
        this->reject();
    } else {
        Start();
    }
    return nRet;    
}

void CDlgScanQRcode::OnTimeOut()
{
    on_pushButton_clicked();
}

int CDlgScanQRcode::Start()
{
    if(m_pCamera)
    {
        m_pCamera->Start();
        //m_Timer.start(1000);
    }
    return 0;
}

int CDlgScanQRcode::Stop()
{
    if(m_pCamera)
    {
        m_Timer.stop();
        m_pCamera->Stop();
    }
    return 0;
}

void CDlgScanQRcode::on_pushButton_clicked()
{
    if(m_pCamera)
    {
        QString szFile =  QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                + QDir::separator() + "CaptureQRCode" + ".png";
        m_pCamera->Capture(szFile.toStdString());
    }
}

void CDlgScanQRcode::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case 0:
        this->setWindowTitle(tr("Scan QR Code"));
        break;
    case 1:
        this->setWindowTitle(tr("Generate QR Code"));
        break;
    default:
        break;
    }
}

void CDlgScanQRcode::on_pbGenerate_clicked()
{
    m_Generate = CQRCode::QRcodeEncodeString(ui->txtContent->toPlainText(), m_Logon);
    if(m_Generate.isNull())
        return;
    ui->lbQRCode->setPixmap(QPixmap::fromImage(m_Generate));
}

void CDlgScanQRcode::on_pbSaveAs_clicked()
{
    if(m_Generate.isNull())
        return;
    QString szFile, szFilter("*.PNG *.BMP *.JPG *.JPEG *.PBM *.PGM *.PPM *.XBM *.XPM");
    szFile = CTool::FileDialog(this, "Generate.PNG",
                               szFilter, tr("Save as"), QFileDialog::AcceptSave);
    if(szFile.isEmpty())
       return; 
    if(!m_Generate.save(szFile))
        LOG_MODEL_ERROR("CDlgScanQRcode", "Save qrencode fail:%s",
                        szFile.toStdString().c_str());
}

void CDlgScanQRcode::on_Cancel_2_clicked()
{
    this->accept();
}

void CDlgScanQRcode::on_pbBrowse_clicked()
{
    QString szFile, szFilter(tr("Image Files (*.PNG *.BMP *.JPG *.JPEG *.PBM *.PGM *.PPM *.XBM *.XPM);;All Files (*.*)"));
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(!szFile.isEmpty())
    {
        if(m_Logon.load(szFile))
        {
            ui->lbLogon->setPixmap(QPixmap::fromImage(m_Logon));
        }
    }
}
