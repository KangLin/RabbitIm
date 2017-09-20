#ifndef DETECTFACES_H
#define DETECTFACES_H

#include <QObject>
#include <opencv2\opencv.hpp>

class CDetectFaces : public QObject
{
    Q_OBJECT
public:
    explicit CDetectFaces(QObject *parent = 0);
    bool DetectFaces(cv::Mat frame, cv::Mat frame_gray, double scale);
    bool DetectFaces(cv::Mat frame, cv::Mat frame_gray);
    
signals:
    
public slots:
    
private:
    cv::CascadeClassifier m_FaceCascade;   //定义人脸分类器
    cv::CascadeClassifier m_EyesCascade;   //定义人眼分类器
    
};

#endif // DETECTFACES_H
