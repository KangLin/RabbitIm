#include "LbsTrack.h"
#include "ui_LbsTrack.h"
#include <QDebug>
#include <math.h>
#include "Nmea.h"
#include <fstream>
#include <QStandardPaths>
#include <QDir>
#include <QNmeaPositionInfoSource>
#include "../Global/Log.h"

CLbsTrack::CLbsTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLbsTrack)
{
    ui->setupUi(this);
    
    m_bStart = false;
    /*m_NmeaSaveFile = 
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + QDir::separator() + "gps.nmea"; //*/
    
#ifdef MOBILE
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
#else
    QNmeaPositionInfoSource* p = new QNmeaPositionInfoSource(
                QNmeaPositionInfoSource::SimulationMode, this);
    m_Source = p;
    if(m_Source)
    {
        m_NmeaFile.setFileName(":/file/gps.nmea");
        if(m_NmeaFile.open(QIODevice::ReadOnly))
            p->setDevice(&m_NmeaFile);
        else
            LOG_MODEL_ERROR("CLbsTrack", "don't open file:%s",
                            m_NmeaFile.fileName().toStdString().c_str());
    }
#endif
    
    if (m_Source) {
        bool check = connect(m_Source,
                             SIGNAL(positionUpdated(QGeoPositionInfo)),
                             this, SLOT(positionUpdated(QGeoPositionInfo)));
        Q_ASSERT(check);
    }
}

CLbsTrack::~CLbsTrack()
{
#ifndef MOBILE
    if(m_NmeaFile.isOpen())
    {
        m_NmeaFile.close();
    }
    if(m_Source)
        delete m_Source;
#endif    
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

    //保存到本地文件中  
    std::ofstream out(m_NmeaSaveFile.toStdString().c_str(), std::ios::app);
    if (out.is_open()) 
        out << CNmea::EncodeGMC(info).c_str() << std::endl;
    out.close();
}

void CLbsTrack::on_pbStart_clicked()
{
    if(m_Source)
    {
        if(m_bStart)
        {   
            m_Source->stopUpdates();
            ui->pbStart->setText(tr("Start"));
        }
        else
        {
            m_Source->startUpdates();
            ui->pbStart->setText(tr("Stop"));
        }
        m_bStart = !m_bStart;
    }
}
