#ifndef LBSMAIN_H
#define LBSMAIN_H

#include <QWidget>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <string>
#include <QFile>

namespace Ui {
class CLbsTrack;
}

class CLbsTrack : public QWidget
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
    QFile m_NmeaFile;//Name模拟文件  
    QString m_NmeaSaveFile;//Nmea保存文件  
};

#endif // LBSMAIN_H
