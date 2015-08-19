#ifndef LBSMOTION_H
#define LBSMOTION_H

#include <QWidget>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QTimer>
//#include <QPropertyAnimation>
#include <QMessageBox>
#include <QRunnable>
#include "LbsPositionLogger.h"

namespace Ui {
class CLbsMotion;
}

class CLbsMotion : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLbsMotion(QWidget *parent = 0);
    ~CLbsMotion();
    
private slots:
    void positionUpdated(const QGeoPositionInfo &info);
    void on_pbStart_clicked();
    void on_pbPause_clicked();
    void on_pbShow_clicked();
    void OnTimeOut();
    
private:
    //从记录文件中上传数据到服务器  
    int onUploadServer();
private slots:
    void slotExitMessageBox(int nRet);
    void on_pbCamera_clicked();
    void slotPhotograph(const QString &szFile);
    
signals:
    void sigExitMessageBox(int nRet);
    
private:
    Ui::CLbsMotion *ui;
    
    QGeoPositionInfoSource *m_Source;
    bool m_bStart;
    bool m_bPause;
    bool m_bHide;

    //轨迹记录文件  
    CLbsPositionLogger* m_pLogger;  
    QString m_szSaveFile;
    QFile m_inFile;//Name模拟文件  
    
    long m_StartTime; //开始时间  
    long m_PauseTime; //暂停时间  
    QTimer m_Timer;   //刷新定时器
    qreal m_Distance;  //运动距离  
    QGeoPositionInfo m_lastPostion;  //上一个位置点，用于计算距离  
    
    QString m_szUrl;
    QString m_szUser;
    QString m_szDevice;

    //QPropertyAnimation m_Animation;//动画提示隐藏，没完成
    QMessageBox m_MessageBox;
    friend class UploadTask;
 
};

class UploadTask : public QRunnable
{
public:
    UploadTask(CLbsMotion* pThis);
    void run();
private:
    CLbsMotion* m_pThis;
};


#endif // LBSMOTION_H
