#include "FrmFaceRecognizer.h"
#include "ui_FrmFaceRecognizer.h"
#include <QDebug>

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
    m_DetectFace.DetectFaces(frame, frame_gray, faces, 6);

    switch (m_Operator) {
    case SAVE:
    {
        std::vector<cv::Rect>::iterator it;
        for(it = faces.begin(); it != faces.end(); it++)
        {
            cv::Mat ROI = frame(*it);
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
        m_DetectFace.Recognizer(frame_gray, label, confidence);
    }
        break;
    default:
        break;
    }
    m_Operator = NO;
    ShowImage(frame);
    //qDebug() << "Width:" << frame.cols << "Height:" << frame.rows;

}

void CFrmFaceRecognizer::ShowImage(cv::Mat image)
{
    cv::Mat frame;
    if(image.cols != 640 || image.rows != 480)
    {
        cv::resize(image, frame, cv::Size(640, 480));
    }
    else
        frame = image;

    cv::cvtColor(frame, m_Frame, cv::COLOR_BGR2RGBA);
    QImage i(m_Frame.data, m_Frame.cols, m_Frame.rows, QImage::Format_RGB32);

    ui->lbShow->setPixmap(QPixmap::fromImage(i));
}

void CFrmFaceRecognizer::on_pbTrain_clicked()
{
    m_Operator = TRAIN;
}

void CFrmFaceRecognizer::on_pbRecgnizer_clicked()
{
    m_Operator = RECOGNIZER;
}
