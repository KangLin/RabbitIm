#ifndef CQRCODE_H
#define CQRCODE_H

#include "Global/Global.h"
#include <QObject>

class CQRCode : public QObject
{
    Q_OBJECT
public:
    explicit CQRCode();
    
    /**
     * @brief 生成二维码图片  
     * @param szString
     * @return 
     */
    static QImage QRcodeEncodeString(const QString &szData,
                                     const QSize &size = QSize(480, 480));
    /**
     * @brief 生成用户信息二维码  
     * @param szId
     * @return 
     */
    static QImage QRcodeEncodeUserInfo(const QString &szInfo);
    /**
     * @brief 生成用户ID二维码  
     * @param szId
     * @return 
     */
    static QImage QRcodeEncodeUserId(const QString &szId);
    /**
     * @brief 识别图片文件  
     * @param szFile
     * @return 
     */
    static QString ProcessQRFile(const QString &szFile);
    /**
     * @brief 识别图片  
     * @param image
     * @return 
     */
    static QString ProcessQImage(QImage image);
    
private:
    static int ProcessMessage(const QString &szMessage);
};

#endif // CQRCODE_H
