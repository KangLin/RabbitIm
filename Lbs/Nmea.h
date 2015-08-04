#ifndef CNMEA_H
#define CNMEA_H

#include <string>
#include <QGeoPositionInfo>

class CNmea
{
public:
    CNmea();

    static std::string EncodeGMC(const QGeoPositionInfo &info);
    static std::string NMEAGPRMCCoord(double coord);
    static std::string NMEACheckSum(std::string msg);
    static bool SendHttpOpenGts(std::string szUrl, std::string szUser,
                                std::string szDevice,
                                const QGeoPositionInfo &info);
};

#endif // CNMEA_H
