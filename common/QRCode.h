#ifndef CQRCODE_H
#define CQRCODE_H

#include "Global/Global.h"
#include <QObject>

class RABBITIM_SHARED_LIBRARY CQRCode : public QObject
{
    Q_OBJECT
public:
    explicit CQRCode();
    
    /**
     * @brief 生成二维码图片  
     * @param szString:生成二维码的字符串  
     * @param inImage:在二维码中间画的图像
     * @return 
     */
    static QImage QRcodeEncodeString(const QString &szData,
                                     const QImage &inImage = QImage(),
                                     const QSize &size = QSize(480, 480));
    
    /**
     * @brief 生成用户信息二维码  
     * @param szInfo:要生成二维码的用户信息    
     * @param inImage:在二维码中间画的图像
     * @return 
     */
    static QImage QRcodeEncodeUserInfo(const QString &szInfo,
                                       const QImage &inImage = QImage());
    /**
     * @brief 生成用户ID二维码  
     * @param szId：要生成二维码的用户ID    
     * @param inImage:在二维码中间画的图像
     * @return 
     */
    static QImage QRcodeEncodeUserId(const QString &szId,
                                     const QImage &inImage = QImage());
    /**
     * @brief 识别图片文件  
     * @param szFile
     * @param szText :识别出来的文本  
     * @return 
     */
    static int ProcessQRFile(const QString &szFile, QString &szText);
    /**
     * @brief 识别图片  
     * @param image
     * @param szText :识别出来的文本  
     * @return >0:解码失败  
     *         =0:解码处理均成功  
     *         <0:处理出错  
     */
    static int ProcessQImage(QImage image, QString &szText);
    //static int ProcessQImage(const std::shared_ptr<CVideoFrame> &frame, QString &szText);

private:
    /**
     * @brief 处理消息  
     * @param szMessage
     * @return 成功返回0，失败返回非0
     */
    static int ProcessMessage(const QString &szMessage);
};

#endif // CQRCODE_H
