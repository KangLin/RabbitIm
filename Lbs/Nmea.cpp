#include "Nmea.h"
#include "curl/curl.h"
#include "../Global/Log.h"
#include <math.h>

CNmea::CNmea()
{
}

/**
 * @brief 
 
 $GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh
 
 $GPRMC,082006.000,A,3852.9276,N,11527.4283,E,0.00,0.0,261009,,*38
 
 <1> UTC时间，hhmmss(时分秒)格式    08 时20 分06 秒
 <2> 定位状态，A=有效定位，V=无效定位
 <3> 纬度ddmm.mmmm(度分)格式(前面的0也将被传输)  北纬38 度52.9276 分
 <4> 纬度半球N(北半球)或S(南半球)
 <5> 经度dddmm.mmmm(度分)格式(前面的0也将被传输)  东经115 度27.4283 分
 <6> 经度半球E(东经)或W(西经)
 <7> 地面速率(000.0~999.9节，前面的0也将被传输)
 <8> 地面航向(000.0~359.9度，以真北为参考基准，前面的0也将被传输)
 <9> UTC日期，ddmmyy(日月年)格式    26日10月09年
 <10> 磁偏角(000.0~180.0度，前面的0也将被传输)
 <11> 磁偏角方向，E(东)或W(西)
 <12> 模式指示(仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效)
 
 经纬度格式分为三种：度、度-分、度-份-秒 
 1.） ddd.ddddd 【度 . 度 格式】的十进制小数部分（5位） 
 2.） ddd°mm.mmm’ 【度 . 分 . 分 格式】的十进制小数部分（3位） 
 3.)   ddd°mm’ss’’ 【度 . 分 . 秒 格式】 Google 使用的是第三种格式  度 分’秒’’
 
 <1> 当前位置的格林尼治时间，即世界时间，与北京时间差8个小时，格式为hhmmss.ms [pos+6]
 <2> 状态, A 为有效位置, V为非有效接收警告，即当前天线视野上方的卫星个数少于3颗。 [pos+17]
 
 注意几点：
 1、当GPS数据有效时第17位（一般情况下，程序里最好是找第二个逗号在取下一位判断）为“A”，无效时为“V”；
 2、GPS有效时，当速度为0时显示0.0（两位数），当速度不为0时小数点前面数据根据情况变化，最大为三位，此处速度单位为节（海里），需要做处理才能得到我们习惯的单位（公里/小时）；
 3、GPS无效时，除了第17位显示V以外，不输入速度，角度数据；
 4、当给GPS复位时第17位为V，不输出速度，角度，时间数据。
 
 * @param info
 * @return 
 */
std::string CNmea::EncodeGMC(const QGeoPositionInfo &info)
{
    std::string szRmc;
    szRmc = "$GPRMC,"
            + info.timestamp().toUTC().toString("HHmmss.zzz").toStdString()
            + ",A," 
            +  NMEAGPRMCCoord(fabs(info.coordinate().latitude()));
    if(info.coordinate().latitude() >= 0)
        szRmc += ",N,";
    else
        szRmc += ",S,";
    szRmc += NMEAGPRMCCoord(fabs(info.coordinate().longitude()));
    if(info.coordinate().longitude() >= 0)
        szRmc += ",E,";
    else
        szRmc += ",W,";
    char buf[10];
    if(info.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        sprintf(buf, "%f,",
                info.attribute(QGeoPositionInfo::GroundSpeed) * 1.94384449);
        szRmc += buf;
    } else
        szRmc += ",";
    if(info.hasAttribute(QGeoPositionInfo::Direction))
    {
        sprintf(buf, "%f,",
                info.attribute(QGeoPositionInfo::Direction));
        szRmc += buf;
    }else
        szRmc += ",";
    
    szRmc += info.timestamp().toUTC().toString("ddMMyy").toStdString();
    szRmc += ",";
    if(info.hasAttribute(QGeoPositionInfo::MagneticVariation))
    {
        sprintf(buf, "%f,", abs(info.attribute(QGeoPositionInfo::MagneticVariation)));
        szRmc += buf;
        if(info.attribute(QGeoPositionInfo::MagneticVariation) >= 0)
            szRmc += "E,";
        else
            szRmc += "W,";
    }else
        szRmc += ",,";
    szRmc += "*" + NMEACheckSum(szRmc);
    return szRmc;
}

std::string CNmea::NMEAGPRMCCoord(double coord) 
{
    // “DDDMM.MMMMM”
    int degrees = (int) coord;
    double minutes = (coord - degrees) * 60;

    char buf[10];
    sprintf(buf, "%d%.5f", degrees, minutes);
    
    return buf;
}

std::string CNmea::NMEACheckSum(std::string msg)
{
    char chk = 0;
    for (int i = 1; i < msg.length(); i++) {
        chk ^= msg.at(i);
    }
    char buf[4];
    sprintf(buf, "%02X", chk);
    return buf;
}

bool CNmea::SendHttpOpenGts(std::string szUrl,
                            std::string szUser,
                            std::string szDevice,
                            const QGeoPositionInfo &info)
{
    CURL *curl;
    CURLcode res;
    
    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, szUrl.c_str());
        
        /* Now specify the POST data */ 
        std::string szData;;
        szData += "id=" + szDevice;
        szData += "&dev=" + szDevice;
        szData += "&acct=" + szUser;
        //OpenGTS 2.5.5 requires batt param or it throws exception...
        szData += "&batt=0&code=0xF020";
        double dbAlt = info.coordinate().altitude();
        if(!qIsNaN(dbAlt))
            szData += "&alt=" + QString::number(dbAlt).toStdString();
        szData += "&gprmc=" + EncodeGMC(info);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szData.c_str());
        
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            LOG_MODEL_ERROR("Nmea", "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    
    if(res != CURLE_OK)
        return false;
    return true;
}
