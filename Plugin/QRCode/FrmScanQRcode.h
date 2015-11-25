#ifndef DLGSCANQRCODE_H
#define DLGSCANQRCODE_H

#include <QFrame>
#include <QTimer>
#include <QCameraImageCapture>
#include "Media/Camera/CameraFactory.h"
#include "Widgets/FrmVideo/FrmVideo.h"

namespace Ui {
class CFrmScanQRcode;
}

class CFrmScanQRcode : public QFrame, CCamera::CHanderFrame
{
    Q_OBJECT
    
public:
    explicit CFrmScanQRcode(QWidget *parent = 0);
    ~CFrmScanQRcode();

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
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent * event);
    
private slots:
    void on_pushBrowse_clicked();
    void on_Cancel_clicked();
    void OnTimeOut();
    
    void on_pushButton_clicked();
    
    void on_tabWidget_currentChanged(int index);
    
    void on_pbGenerate_clicked();
    
    void on_pbSaveAs_clicked();
    
    void on_Cancel_2_clicked();
    
    void on_pbBrowse_clicked();
    
private:
    Ui::CFrmScanQRcode *ui;
    CCamera* m_pCamera;
    CFrmVideo m_Play;
    QCameraImageCapture* m_pCameraImageCapture;
    QTimer m_Timer;
    QImage m_Generate;
    QImage m_Logon;
};

#endif // DLGSCANQRCODE_H
