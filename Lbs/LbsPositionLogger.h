#ifndef CLBSPOSITIONLOGGER_H
#define CLBSPOSITIONLOGGER_H

#include <QtPositioning/qgeopositioninfosource.h>
#include <QFile>
#include <QTimer>
#include <QGeoPositionInfo>
#include <QString>
#include <fstream>
#include <list>
#include <string>

/**
 * @brief 自定义格式的位置存储与读取  
 */
class CLbsPositionLogger : public QGeoPositionInfoSource
{
    Q_OBJECT

public:
    CLbsPositionLogger(QObject *parent = 0);
    CLbsPositionLogger(const char* pszFile, QObject *parent = 0);
    virtual ~CLbsPositionLogger();

    virtual QGeoPositionInfo lastKnownPosition(bool fromSatellitePositioningMethodsOnly = false) const;

    virtual PositioningMethods supportedPositioningMethods() const;
    virtual int minimumUpdateInterval() const;
    virtual Error error() const;

    /**
     * @brief 把位置信息写入文件  
     * @param info：位置信息  
     * @return 成功返回0；失败返回非0  
     */
    virtual int Write(const QGeoPositionInfo &info);
    virtual int Write(std::list<QGeoPositionInfo> &lstInfo);
    
public slots:
    virtual void startUpdates();
    virtual void stopUpdates();

    virtual void requestUpdate(int timeout = 1000);

private slots:
    void readNextPosition();

private:
    QString m_szFile;
    QFile m_logFile;
    QTimer m_timer;
    QGeoPositionInfo m_lastPosition;
};

#endif // CLBSPOSITIONLOGGER_H
