#ifndef CNMEA_H
#define CNMEA_H

#include <string>
#include <QGeoPositionInfo>
#include <list>

class CNmea
{
public:
    CNmea();

    static std::string EncodeGMC(const QGeoPositionInfo &info);
    static std::string NMEAGPRMCCoord(double coord);
    static std::string NMEACheckSum(std::string msg);
    static int SendHttpOpenGts(const std::string &szUrl,
                               const std::string &szUser,
                               const std::string &szDevice,
                               const QGeoPositionInfo &info);
    static int GetHttpOpenGts(std::list<QGeoPositionInfo> &lstInfo,
                              const std::string &szUrl,
                              const std::string &szUser,
                              const std::string &szPassword,
                              const std::string &szDevice,
                              const unsigned int limit = 100,
                              const unsigned long startTime = 0,
                              const unsigned long endTime = 0
                              );
};

#endif // CNMEA_H
