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
#include <QQuickItem>

CLbsTrack::CLbsTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLbsTrack)
{
    ui->setupUi(this);
    ui->quickWidget->setSource(QStringLiteral("qrc:///qml/Map.qml"));
    
    m_bStart = false;
    m_Source = NULL;
    m_pLogger = NULL;
    
    /*默认 opents gprmc 上传   
    m_szUrl = "http://182.254.185.29:8080/gprmc/Data";
    m_szUser = "root";
    m_szDevice = "123456";//*/

#ifdef MOBILE
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
#else
    //*
    QNmeaPositionInfoSource* p = new QNmeaPositionInfoSource(
                QNmeaPositionInfoSource::SimulationMode, this);
    m_Source = p;
    if(m_Source)
    {
        m_inFile.setFileName("D:/temp/gps.nmea");
        if(m_inFile.open(QIODevice::ReadOnly))
        {   
            p->setDevice(&m_inFile);
            //p->setUpdateInterval(1000);
        }
        else
            LOG_MODEL_ERROR("CLbsTrack", "don't open file:%s",
                            m_inFile.fileName().toStdString().c_str());
    }//*/
    
    /*
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
    
    if(m_pLogger)
    {
        m_pLogger->CloseFile();
        delete m_pLogger;
    }
    delete ui;
}

void CLbsTrack::positionUpdated(const QGeoPositionInfo &info)
{
    if(!info.isValid())
    {   
        ui->txtLogger->append("QGeoPositionInfo is invalid\n");
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
    ui->txtLogger->append(szMsg);

    //保存到本地文件中  
    if(m_pLogger)
        m_pLogger->Write(info);

    //上传到opengts gprmce服务器  
    if(!(m_szUrl.isEmpty() || m_szUser.isEmpty() || m_szDevice.isEmpty()))
        CNmea::SendHttpOpenGts(m_szUrl.toStdString().c_str(),
                           m_szUser.toStdString().c_str(),
                           m_szDevice.toStdString().c_str(),
                           info);
    //在地图上显示轨迹  
    QQuickItem * pRoot = ui->quickWidget->rootObject();
    QObject* pMap = pRoot->findChild<QObject*>("objNameMap");
    if(pMap)
    {
        pMap->setProperty("center", QVariant::fromValue(info.coordinate()));
        QMetaObject::invokeMethod(pMap, "addPolylinePoint",
                   Q_ARG(QVariant, QVariant::fromValue(info.coordinate())));
    }
}

void CLbsTrack::on_pbStart_clicked()
{
#ifndef MOBILE
    /*测试opengts gprmc 下载gps数据  
    std::list<QGeoPositionInfo> info;
    CNmea::GetHttpOpenGts(info, "http://182.254.185.29:8080/", "root", "123456", "123456");
    std::list<QGeoPositionInfo>::iterator it;
    for(it = info.begin(); it != info.end(); it++)
    {
        positionUpdated(*it);
    }
    return;//*/
#endif
    
    if(m_Source)
    {
        if(m_bStart)
        {   
            m_Source->stopUpdates();
            ui->pbStart->setText(tr("Start"));
            
            if(m_pLogger)
            {
                m_pLogger->CloseFile();
                delete m_pLogger;
                m_pLogger = NULL;
            }
        }
        else
        {
            if(NULL == m_pLogger)
            {
                QString szFile =
                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                        + QDir::separator() + "RabbitMotion";
                QDir d;
                if(!d.exists(szFile))
                    d.mkdir(szFile);
                szFile += QDir::separator()
                     + QDateTime::currentDateTime().toString("yyyyMMddHHmmss")
                     + ".dat";
                m_pLogger = new CLbsPositionLogger(szFile.toStdString().c_str());
                if(m_pLogger)
                    m_pLogger->OpenFile();
            }

            m_Source->startUpdates();
            ui->pbStart->setText(tr("Stop"));
        }
        m_bStart = !m_bStart;
    }
    
}
