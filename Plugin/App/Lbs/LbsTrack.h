#ifndef LBSMAIN_H
#define LBSMAIN_H

#include <QFrame>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <string>
#include <QFile>
#include "LbsPositionLogger.h"

namespace Ui {
class CLbsTrack;
}

class CLbsTrack : public QFrame
{
    Q_OBJECT
    
public:
    explicit CLbsTrack(QWidget *parent = 0);
    ~CLbsTrack();

private slots:
    void positionUpdated(const QGeoPositionInfo &info);

    void on_pbStart_clicked();
    
private:
    Ui::CLbsTrack *ui;
    QGeoPositionInfoSource *m_Source;
    bool m_bStart;

    CLbsPositionLogger* m_pLogger;    
    QFile m_inFile;//Name模拟文件  
    //用于opengts gprmc 传送  
    QString m_szUrl;   //opengts gprmc服务器URL  
    QString m_szUser;  //用户名  
    QString m_szDevice;//设备ID  
    
};

#endif // LBSMAIN_H
