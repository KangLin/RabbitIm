#ifndef DLGSCANQRCODE_H
#define DLGSCANQRCODE_H

#include <QFrame>
#include <QTimer>
#include <QCamera>
#include "Widgets/FrmVideo/FrmDisplay.h"
#include "Widgets/FrmVideo/CameraQtCaptureVideoFrame.h"
#include "qrcode_export.h"

namespace Ui {
class CFrmScanQRcode;
}

class QRCODE_EXPORT CFrmScanQRcode : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmScanQRcode(QWidget *parent = nullptr);
    ~CFrmScanQRcode();

    //识别二维码文件  
    int ProcessQRFile(QString szFile);
    
private:
    int Start();
    int Stop();
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent * event);
    
private slots:
    /**
     * @brief 重载此函数进行帧处理  
     * @param frame 捕获的帧  
     * @return 
     */
    virtual int slotCaptureFrame(const QImage &frame);

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
    QCamera *m_pCamera;
    CFrmDisplay m_Play;
    CCameraQtCaptureVideoFrame m_CaptureVideoFrame;
    QTimer m_Timer;
    QImage m_Generate;
    QImage m_Logon;
    QImage m_CaptureImage;
};

#endif // DLGSCANQRCODE_H
