#ifndef DLGSCANQRCODE_H
#define DLGSCANQRCODE_H

#include <QDialog>
#include <QTimer>
#include <QCameraImageCapture>
#include "Media/Camera/CameraFactory.h"
#include "Widgets/FrmVideo/FrmVideo.h"

namespace Ui {
class CDlgScanQRcode;
}

class CDlgScanQRcode : public QDialog, CCamera::CHanderFrame
{
    Q_OBJECT
    
public:
    explicit CDlgScanQRcode(QWidget *parent = 0);
    ~CDlgScanQRcode();

    //识别二维码文件  
    int ProcessQRFile(QString szFile);

    /**
     * @brief 重载此函数进行帧处理  
     * @param frame 捕获的帧  
     * @return 
     */
    virtual int OnFrame(const std::shared_ptr<CVideoFrame> frame);
    virtual int OnCapture(const std::string szFile);
    
private:
    int Start();
    int Stop();
    
private slots:
    void on_pushBrowse_clicked();
    void on_Cancel_clicked();
    void OnTimeOut();
    
private:
    Ui::CDlgScanQRcode *ui;
    CCamera* m_pCamera;
    CFrmVideo m_Play;
    QCameraImageCapture* m_pCameraImageCapture;
    QTimer m_Timer;
};

#endif // DLGSCANQRCODE_H
