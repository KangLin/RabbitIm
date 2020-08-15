#include "FrmFaceRecognizer.h"
#include "ui_FrmFaceRecognizer.h"
#include <QDebug>
#include "Tool.h"

CFrmFaceRecognizer::CFrmFaceRecognizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmFaceRecognizer)
{
    ui->setupUi(this);

    bool check = connect(&m_CapThread, SIGNAL(sigCaptured(cv::Mat)),
            this, SLOT(slotCaptured(cv::Mat)));
    Q_ASSERT(check);
    
    m_CapThread.start();
}

CFrmFaceRecognizer::~CFrmFaceRecognizer()
{
    m_CapThread.Stop();
    delete ui;
    QThread::msleep(500);
}

void CFrmFaceRecognizer::on_pbSave_clicked()
{
    m_Operator = SAVE;
}

void CFrmFaceRecognizer::slotCaptured(cv::Mat frame)
{
    cv::Mat frame_gray;
    std::vector<cv::Rect> faces;
    //m_DetectFace.DetectFaces(frame, frame_gray);
    m_DetectFace.DetectFaces(frame, frame_gray, faces, 4);
    
    switch (m_Operator) {
    case SAVE:
    {
        std::vector<cv::Rect>::iterator it;
        for(it = faces.begin(); it != faces.end(); it++)
        {
            cv::Rect r = *it;
            cv::Mat ROI = frame_gray(r);
            cv::Mat img(50, 120,  CV_8UC1);
            cv::resize(ROI, img, img.size(),  0, 0, cv::INTER_LINEAR);
            CTool::ShowImage(ui->lbGray, img);
            ROI = frame(r);
            CTool::ShowImage(ui->lbHist, ROI);
            qDebug() << "ROI width:" << ROI.cols << " Height:" << ROI.rows;
            m_DetectFace.AddImage(ROI, ui->leLabel->text().toInt());
        }
     
    }
           break;
    case TRAIN:
        m_DetectFace.Train();
        break;
    case RECOGNIZER:
    {
        int label = -1;
        double confidence = 0.0;
        std::vector<cv::Rect>::iterator it;
        for(it = faces.begin(); it != faces.end(); it++)
        {
            m_DetectFace.Recognizer(frame_gray(*it), label, confidence);
        }
    }
        break;
    default:
        break;
    }
    m_Operator = NO;//*/
    CTool::ShowImage(ui->lbShow, frame);
    //qDebug() << "Width:" << frame.cols << "Height:" << frame.rows;

}

void CFrmFaceRecognizer::on_pbTrain_clicked()
{
    m_Operator = TRAIN;
}

void CFrmFaceRecognizer::on_pbRecgnizer_clicked()
{
    m_Operator = RECOGNIZER;
}
