#include "DetectFaces.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDebug>

CDetectFaces::CDetectFaces(QObject *parent) : QObject(parent)
{  
    cv::String szCascades = "D:\\source\\RabbitIm\\ThirdLibrary\\windows_msvc\\etc\\haarcascades\\";
    cv::String szFaceCascade = szCascades + "haarcascade_frontalface_alt2.xml";
    m_FaceCascade.load(szFaceCascade);
    //cv::String eyes_cascade_name = szCascades + "haarcascade_eye_tree_eyeglasses.xml";
    cv::String eyes_cascade_name = szCascades + "haarcascade_eye.xml";
    m_EyesCascade.load(eyes_cascade_name);

    m_Model = cv::face::LBPHFaceRecognizer::create();
}

bool CDetectFaces::DetectFaces(cv::Mat frame, cv::Mat &frame_gray)
{
    bool bFind = false;
    double t = 0;
    std::vector<cv::Rect> faces;
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);
    t = (double)cvGetTickCount();
    m_FaceCascade.detectMultiScale(frame_gray,
                                   faces,
                                   1.1,
                                   3,
                                   CV_HAAR_DO_ROUGH_SEARCH,
                                   cv::Size(1, 1),
                                   cv::Size(50,50));
    //t = (double)cvGetTickCount() - t;
    //qDebug() << "Detection face time = " << t / ((double)cvGetTickFrequency() * 1000) << "ms";
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::rectangle(frame, faces[i], cv::Scalar(0,0,255), 2, 8, 0);
        
        cv::Mat faceROI = frame_gray(faces[i]);
        std::vector<cv::Rect> eyes;
        
        t = (double)cvGetTickCount() - t;
        //-- In each face, detect eyes
        m_EyesCascade.detectMultiScale(faceROI, eyes, 1.1, 3,
                                       CV_HAAR_DO_ROUGH_SEARCH,
                                       cv::Size(1, 1), cv::Size(50, 50));
        //t = (double)cvGetTickCount() - t;
        //qDebug() << "Detection eye time = " << t / ((double)cvGetTickFrequency() * 1000) << "ms";        
        for (size_t j = 0; j < eyes.size(); j++)
        {
            cv::Rect rect(faces[i].x + eyes[j].x,
                          faces[i].y + eyes[j].y,
                          eyes[j].width,
                          eyes[j].height);
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2, 8, 0);
            bFind = true;
        }
    }
    return bFind;
}

bool CDetectFaces::DetectFaces(cv::Mat frame, cv::Mat &frame_gray, double scale)
{
    bool bFind = false;
    double t = 0;
    std::vector<cv::Rect> faces;
    cv::Mat smallImg(cvRound(frame.rows / scale), cvRound(frame.cols / scale), CV_8UC1);
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    resize(frame_gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);

    cv::equalizeHist(smallImg, smallImg);
    //t = (double)cvGetTickCount();
    m_FaceCascade.detectMultiScale(smallImg,
                                   faces,
                                   1.1,
                                   3,
                                   CV_HAAR_DO_CANNY_PRUNING
                                   |CV_HAAR_FIND_BIGGEST_OBJECT
                                   |CV_HAAR_DO_ROUGH_SEARCH
                                   |CV_HAAR_SCALE_IMAGE);
    //t = (double)cvGetTickCount() - t;
    //qDebug() << "Detection face time = " << t / ((double)cvGetTickFrequency() * 1000) << "ms";
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::Rect r;
        r.x = cvRound(faces[i].x * scale);
        r.y = cvRound(faces[i].y * scale);
        r.width = cvRound(faces[i].width * scale);
        r.height = cvRound(faces[i].height * scale);
        cv::rectangle(frame, r, cv::Scalar(0,0,255), 2, 8, 0);
        
        cv::Mat faceROI = smallImg(faces[i]);
        frame_gray = faceROI.clone();
        std::vector<cv::Rect> eyes;
        
        //-- In each face, detect eyes
        //t = (double)cvGetTickCount();
        m_EyesCascade.detectMultiScale(faceROI,
                                       eyes,
                                       1.1,
                                       3,
                                       CV_HAAR_DO_CANNY_PRUNING
                                       |CV_HAAR_FIND_BIGGEST_OBJECT
                                       |CV_HAAR_DO_ROUGH_SEARCH
                                       |CV_HAAR_SCALE_IMAGE);
        //t = (double)cvGetTickCount() - t;
        //qDebug() << "Detection eye time = " << t / ((double)cvGetTickFrequency() * 1000) << "ms";
        for (size_t j = 0; j < eyes.size(); j++)
        {
            cv::Rect rect(cvRound((faces[i].x + eyes[j].x) * scale),
                          cvRound((faces[i].y + eyes[j].y) * scale),
                          cvRound(eyes[j].width * scale),
                          cvRound(eyes[j].height * scale));
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2, 8, 0);
            bFind = true;
        }
    }
    
    return bFind;
}

int CDetectFaces::AddImage(cv::Mat image, int lable)
{
    m_Images.push_back(image);
    m_Lables.push_back(lable);
    return 0;
}

int CDetectFaces::Train()
{
    int nRet = 0;
    double t = 0;
    t = (double)cvGetTickCount();
    m_Model->train(m_Images, m_Lables);
    t = (double)cvGetTickCount() - t;
    qDebug() << "Detection eye time = " << t / ((double)cvGetTickFrequency() * 1000) << "ms";    
    m_Model->save("MyFaceLBPHModel.xml");
    return nRet;
}

int CDetectFaces::Recognizer(cv::Mat image, CV_OUT int &label, CV_OUT double &confidence)
{
    m_Model->predict(image, label, confidence);
    qDebug() << "label:" << label << " confidence:" << confidence;
    return 0;
}
