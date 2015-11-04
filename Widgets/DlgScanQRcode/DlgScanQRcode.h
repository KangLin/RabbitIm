#ifndef DLGSCANQRCODE_H
#define DLGSCANQRCODE_H

#include <QDialog>
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
    
private slots:
    void on_pushBrowse_clicked();
    
    void on_Cancel_clicked();
    
private:
    Ui::CDlgScanQRcode *ui;
    CCamera* m_pCamera;
    CFrmVideo m_Play;
};

#endif // DLGSCANQRCODE_H
