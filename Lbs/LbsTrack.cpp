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
#include "LbsPositionLogger.h"

CLbsTrack::CLbsTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLbsTrack)
{
    ui->setupUi(this);
    
    m_bStart = false;
    //*默认 nmea 保存文件  
    m_SaveFile = 
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + QDir::separator() + "gps.nmea"; //*/
    /*默认 opents gprmc 上传   
    m_szUrl = "http://182.254.185.29:8080/gprmc/Data";
    m_szUser = "root";
    m_szDevice = "123456";//*/

#ifdef MOBILE
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
#else
    /*
    QNmeaPositionInfoSource* p = new QNmeaPositionInfoSource(
                QNmeaPositionInfoSource::SimulationMode, this);
    m_Source = p;
    if(m_Source)
    {
        m_inFile.setFileName(":/file/gps.nmea");
        if(m_inFile.open(QIODevice::ReadOnly))
        {   
            p->setDevice(&m_inFile);
            //p->setUpdateInterval(1000);
        }
        else
            LOG_MODEL_ERROR("CLbsTrack", "don't open file:%s",
                            m_inFile.fileName().toStdString().c_str());
    }//*/
    
    //*
    CLbsPositionLogger* p = new CLbsPositionLogger(
                "d:/gps.txt");
    m_Source = p;//*/
    
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
    if(m_inFile.isOpen())
    {
        m_inFile.close();
    }
    if(m_Source)
        delete m_Source;
#endif    
    delete ui;
}

void CLbsTrack::positionUpdated(const QGeoPositionInfo &info)
{
    if(!info.isValid())
    {   
        ui->textBrowser->append(tr("QGeoPositionInfo is invalid\n"));
        return;
    }

    QString szMsg;
    szMsg = "latitude:" + QString::number(info.coordinate().latitude())
          + "\nlongitude:" + QString::number(info.coordinate().longitude())
          + "\naltitude:" + QString::number(info.coordinate().altitude())
          + "\ntime:" + info.timestamp().toString()
          + "\nspeed:" + QString::number(info.attribute(QGeoPositionInfo::GroundSpeed))
          + "\nVerticalSpeed:" + QString::number(info.attribute(QGeoPositionInfo::VerticalSpeed))
          + "\nDirection:" + QString::number(info.attribute(QGeoPositionInfo::Direction))
          + "\nHorizontalAccuracy:" + QString::number(info.attribute(QGeoPositionInfo::HorizontalAccuracy))
          + "\nVerticalAccuracy:" + QString::number(info.attribute(QGeoPositionInfo::VerticalAccuracy))
          + "\n" + QString::number(info.attribute(QGeoPositionInfo::MagneticVariation));
    szMsg += "\n" + QString(CNmea::EncodeGMC(info).c_str());
    szMsg += "\n";
    ui->textBrowser->append(szMsg);

    //保存到本地文件中  
    CLbsPositionLogger logger(m_SaveFile.toStdString().c_str());
    logger.startUpdates();
    logger.Write(info);
    logger.stopUpdates();

    //上传到opengts gprmce服务器  
    if(!(m_szUrl.isEmpty() || m_szUser.isEmpty() || m_szDevice.isEmpty()))
        CNmea::SendHttpOpenGts(m_szUrl.toStdString().c_str(),
                           m_szUser.toStdString().c_str(),
                           m_szDevice.toStdString().c_str(),
                           info);
}

void CLbsTrack::on_pbStart_clicked()
{
    /*测试opengts gprmc 下载gps数据  
    std::list<QGeoPositionInfo> info;
    CNmea::GetHttpOpenGts(info, "http://182.254.185.29:8080/", "root", "123456", "123456");
    std::list<QGeoPositionInfo>::iterator it;
    for(it = info.begin(); it != info.end(); it++)
    {
        positionUpdated(*it);
    }
    return;//*/
    
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
