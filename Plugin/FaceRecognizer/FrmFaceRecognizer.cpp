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
    
}

void CFrmFaceRecognizer::slotCaptured(cv::Mat frame)
{
    cv::Mat frame_gray;
    m_DetectFace.DetectFaces(frame, frame_gray, 6);
    ShowImage(frame);
    qDebug() << "Width:" << frame.cols << "Height:" << frame.rows;

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
    
    cv::cvtColor(frame, m_frame, cv::COLOR_BGR2RGBA);
    QImage i(m_frame.data, m_frame.cols, m_frame.rows, QImage::Format_RGB32);

    ui->lbShow->setPixmap(QPixmap::fromImage(i));
}
