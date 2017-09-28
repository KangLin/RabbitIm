#ifndef DETECTFACES_H
#define DETECTFACES_H

#include <QObject>
#include "opencv2\opencv.hpp"
#include "opencv2/face.hpp"

class CDetectFaces : public QObject
{
    Q_OBJECT
public:
    explicit CDetectFaces(QObject *parent = 0);
    bool DetectFaces(cv::Mat frame, cv::Mat &frame_gray, CV_OUT std::vector<cv::Rect>&faces, double scale);
    bool DetectFaces(cv::Mat frame, cv::Mat &frame_gray);

    int AddImage(cv::Mat image, int lable);
    int Train();
    int Recognizer(cv::Mat image, int &label, double &confidence);
    
signals:

public slots:

private:
    cv::CascadeClassifier m_FaceCascade;   //定义人脸分类器
    cv::CascadeClassifier m_EyesCascade;   //定义人眼分类器
    
    cv::Ptr<cv::face::LBPHFaceRecognizer> m_Model;

    std::vector<cv::Mat> m_Images;
    std::vector<int> m_Lables;
};

#endif // DETECTFACES_H
