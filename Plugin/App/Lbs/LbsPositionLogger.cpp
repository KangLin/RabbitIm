#include "LbsPositionLogger.h"
#include <QDateTime>
#include "Global/Log.h"
#include "Nmea.h"

CLbsPositionLogger::CLbsPositionLogger(QObject *parent)
    : QGeoPositionInfoSource(parent)
{    
}

CLbsPositionLogger::CLbsPositionLogger(const char *pszFile, QObject *parent)
    : QGeoPositionInfoSource(parent)
{
    bool check = connect(&m_timer, SIGNAL(timeout()),
                         SLOT(readNextPosition()));
    Q_ASSERT(check);
    m_szFile = pszFile;
}

CLbsPositionLogger::~CLbsPositionLogger()
{
}

QGeoPositionInfo CLbsPositionLogger::lastKnownPosition(bool /*fromSatellitePositioningMethodsOnly*/) const
{
    return m_lastPosition;
}

CLbsPositionLogger::PositioningMethods CLbsPositionLogger::supportedPositioningMethods() const
{
    return AllPositioningMethods;
}

int CLbsPositionLogger::minimumUpdateInterval() const
{
    return 500;
}

void CLbsPositionLogger::startUpdates()
{
    int interval = updateInterval();
    if (interval < minimumUpdateInterval())
        interval = minimumUpdateInterval();

    m_timer.start(interval);
}

void CLbsPositionLogger::stopUpdates()
{
    m_timer.stop();
}

int CLbsPositionLogger::OpenFile(const char *pszFile)
{
    if(pszFile)
        m_szFile = pszFile;
    m_logFile.setFileName(m_szFile);
    if(!m_logFile.open(QIODevice::ReadOnly | QIODevice::WriteOnly))
    {
        LOG_MODEL_ERROR("CLbsPositionLogger", "open file fail:%s",
                        m_szFile.toStdString().c_str());
        return -1;
    }

    return 0;
}

int CLbsPositionLogger::CloseFile()
{
    m_logFile.close();    
    return 0;
}

void CLbsPositionLogger::requestUpdate(int timeout)
{
    Q_UNUSED(timeout);
    // For simplicity, ignore timeout - assume that if data is not available
    // now, no data will be added to the file later
    if (m_logFile.canReadLine())
        readNextPosition();
    else
        emit updateTimeout();
}

QGeoPositionInfoSource::Error CLbsPositionLogger::error() const
{
    return QGeoPositionInfoSource::NoError;
}

int CLbsPositionLogger::Write(std::list<QGeoPositionInfo> &lstInfo)
{
    std::list<QGeoPositionInfo>::iterator it;
    for(it = lstInfo.begin(); it != lstInfo.end(); it++)
    {
        int nRet = Write(*it);
        if(nRet)
            return nRet;
    }
    return 0;
}

int CLbsPositionLogger::Write(const QGeoPositionInfo &info)
{
    int nRet = 0;

    QString szContext;
    szContext = QString::number(info.timestamp().toUTC().toTime_t()) + ","
            + QString::number(info.coordinate().latitude()) + ","
            + QString::number(info.coordinate().longitude()) + ",";
    if(!qIsNaN(info.coordinate().altitude()))
        szContext += QString::number(info.coordinate().altitude());
    szContext += ",";
    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
        szContext += QString::number(info.attribute(QGeoPositionInfo::HorizontalAccuracy));
    szContext += ",";
    if(info.hasAttribute(QGeoPositionInfo::Direction))
        szContext += QString::number(info.attribute(QGeoPositionInfo::Direction));
    szContext += ",";
    if(info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        szContext += QString::number(info.attribute(QGeoPositionInfo::GroundSpeed));
    szContext += "\n";
    m_logFile.write(szContext.toStdString().c_str(), szContext.length());
    return nRet;
}

void CLbsPositionLogger::readNextPosition()
{
    QByteArray line = m_logFile.readLine().trimmed();
    if (line.isEmpty()) {
        LOG_MODEL_ERROR("CLbsPositionLogger", "no readlien");
        return;
    }
    
    QList<QByteArray> data = line.split(',');
    
    QGeoCoordinate coordinate;
    QGeoPositionInfo info;
    
    bool hasTimestamp = false;
    QDateTime timestamp = QDateTime::fromTime_t(data.value(0).toLong(&hasTimestamp), Qt::UTC);
    if(hasTimestamp && timestamp.isValid())
        info.setTimestamp(timestamp);
    double latitude;
    bool hasLatitude = false;
    latitude = data.value(1).toDouble(&hasLatitude);
    if(hasLatitude)
        coordinate.setLatitude(latitude);
    double longitude;
    bool hasLongitude = false;
    longitude = data.value(2).toDouble(&hasLongitude);
    if(hasLongitude)
        coordinate.setLongitude(longitude);
    double altitude;
    bool hasAltitude = false;
    altitude = data.value(3).toDouble(&hasAltitude);
    if(hasAltitude)
        coordinate.setAltitude(altitude);
    info.setCoordinate(coordinate);
    
    double HorizontalAccuracy;
    bool hasHorizontalAccuracy = false;
    HorizontalAccuracy = data.value(4).toDouble(&hasHorizontalAccuracy);
    if(hasHorizontalAccuracy)
        info.setAttribute(QGeoPositionInfo::HorizontalAccuracy, HorizontalAccuracy);
    double Direction;
    bool hasDirection = false;
    Direction = data.value(5).toDouble(&hasDirection);
    if(hasDirection)
        info.setAttribute(QGeoPositionInfo::Direction, Direction);
    double GroundSpeed;
    bool hasGroundSpeed = false;
    GroundSpeed = data.value(6).toDouble(&hasGroundSpeed);
    if(hasGroundSpeed)
        info.setAttribute(QGeoPositionInfo::GroundSpeed, GroundSpeed);
    if (info.isValid()) {
        m_lastPosition = info;
        emit positionUpdated(info);
    }
}

int CLbsPositionLogger::UploadServer(QString szUrl, QString szUser,
                                     QString szPassword, QString szDevice)
{
    int nRet = 0;
    this->OpenFile();
    while(!m_logFile.atEnd())
    {
        readNextPosition();
        nRet = CNmea::SendHttpOpenGts(szUrl.toStdString(),
                                          szUser.toStdString(),
                                          szDevice.toStdString(),
                                          m_lastPosition);
        if(nRet)
         break;
    }
    this->CloseFile();
    return nRet;
}
