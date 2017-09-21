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
    
    void on_pbTrain_clicked();
    
    void on_pbRecgnizer_clicked();
    
private:
    Ui::CFrmFaceRecognizer *ui;
    
    CCapThread m_CapThread;
    CDetectFaces m_DetectFace;
    
    enum __OPERATOR
    {
        NO,
        SAVE,
        TRAIN,
        RECOGNIZER
    };
    __OPERATOR m_Operator;
    
    cv::Mat m_Frame;
    void ShowImage(cv::Mat image);
};

#endif // FRMFACERECOGNIZER_H
