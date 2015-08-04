#include "LbsTrack.h"
#include "ui_LbsTrack.h"
#include <QDebug>
#include <math.h>
#include "Nmea.h"

CLbsTrack::CLbsTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLbsTrack)
{
    ui->setupUi(this);
    
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_Source) {
        bool check = connect(m_Source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        Q_ASSERT(check);
        //TODO:
        m_Source->startUpdates();
    }
}

CLbsTrack::~CLbsTrack()
{
    delete ui;
}

void CLbsTrack::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "Position updated:" << info;
    QString szMsg;
    szMsg = "latitude:" + QString::number(info.coordinate().latitude())
          + "\nlongitude:" + QString::number(info.coordinate().longitude())
          + "\naltitude:" + QString::number(info.coordinate().altitude())
          + "\ntime:" + info.timestamp().toString()
          + "\nspeed:" + QString::number(info.attribute(QGeoPositionInfo::GroundSpeed))
          + "\nDirection:" + QString::number(info.attribute(QGeoPositionInfo::Direction))
          + "\nHorizontalAccuracy" + QString::number(info.attribute(QGeoPositionInfo::HorizontalAccuracy));
    szMsg += "\n" + QString(CNmea::EncodeGMC(info).c_str());
    szMsg += "\n";
    ui->textBrowser->append(szMsg);         
}
