#include "DlgScanQRcode.h"
#include "ui_DlgScanQRcode.h"
#include "Tool.h"
#include "QRCode.h"
#include <QPainter>

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

    m_pCamera = CCameraFactory::Instance()->GetCamera(0);
    if(m_pCamera)
    {
        m_pCamera->Open(this);
        m_pCamera->Start();
    } else {
        ui->lbText->setText(tr("The camera does not exist."));
    }
}

CDlgScanQRcode::~CDlgScanQRcode()
{
    if(m_pCamera)
    {
        m_pCamera->Stop();
        m_pCamera->Close();
    }
    delete ui;
}

void CDlgScanQRcode::on_pushBrowse_clicked()
{
    if(m_pCamera)
        m_pCamera->Stop();

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
    if(m_pCamera)
        m_pCamera->Start();
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
    int nRet = CQRCode::ProcessQImage(frame, szText);
    if(!szText.isEmpty())
        ui->lbText->setText(szText);
    if(nRet <= 0)
    {
        if(m_pCamera)
            m_pCamera->Stop();
        this->reject();
    }
    m_Play.slotDisplayRemoteVideo(frame);
    return 0;
}
