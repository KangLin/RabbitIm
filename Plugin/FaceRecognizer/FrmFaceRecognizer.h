#ifndef FRMFACERECOGNIZER_H
#define FRMFACERECOGNIZER_H

#include <QWidget>
#include "CapThread.h"
#include "DetectFaces.h"

namespace Ui {
class CFrmFaceRecognizer;
}

class CFrmFaceRecognizer : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmFaceRecognizer(QWidget *parent = 0);
    ~CFrmFaceRecognizer();
    
private slots:
    void on_pbSave_clicked();
    void slotCaptured(cv::Mat frame);
    
private:
    Ui::CFrmFaceRecognizer *ui;
    
    CCapThread m_CapThread;
    CDetectFaces m_DetectFace;
    cv::Mat m_frame;
    
    void ShowImage(cv::Mat image);
};

#endif // FRMFACERECOGNIZER_H
