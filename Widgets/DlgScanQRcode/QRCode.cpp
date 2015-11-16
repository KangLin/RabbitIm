#include "QRCode.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QPainter>
#include "MainWindow.h"

#ifdef RABBITIM_USE_LIBQRENCODE
#include "qrencode.h"
#endif

#ifdef RABBITIM_USE_QZXING
    #include "QZXing.h"
#endif

#define RABBITIM_ID "rabbitim://id/"
#define RABBITIM_USERINFO "rabbitim://userinfo/"

CQRCode::CQRCode() : QObject()
{    
}

//http://stackoverflow.com/questions/21400254/how-to-draw-a-qr-code-with-qt-in-native-c-c
QImage CQRCode::QRcodeEncodeString(const QString &szData, const QImage &inImage, const QSize &size)
{
#ifdef RABBITIM_USE_LIBQRENCODE
    QImage image(size, QImage::Format_RGB32);
    QPainter painter(&image);

    //NOTE: I have hardcoded some parameters here that would make more sense as variables.
    // ECLEVEL_M is much faster recognizable by barcodescanner any any other type
    // https://fukuchi.org/works/qrencode/manual/qrencode_8h.html#a4cebc3c670efe1b8866b14c42737fc8f
    // any mode other than QR_MODE_8 or QR_MODE_KANJI results in EINVAL. First 1 is version, second is case sensitivity
    QRcode* qr = QRcode_encodeString(szData.toStdString().c_str(), 1, QR_ECLEVEL_M, QR_MODE_8, 1);
    if (qr != nullptr)
    {
        QColor fg("black");
        QColor bg("white");
        painter.setBrush(bg);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, size.width(), size.height());
        painter.setBrush(fg);
        const int s = qr->width > 0 ? qr->width : 1;
        const double w = size.width();
        const double h = size.height();
        const double aspect = w / h;
        const double scale = ((aspect > 1.0) ? h : w) / s;

        for (int y = 0; y < s; y++)
        {
            const int yy = y * s;
            for (int x = 0; x < s; x++)
            {
                const int xx = yy + x;
                const unsigned char b = qr->data[xx];
                if (b & 0x01)
                {
                    const double rx1 = x * scale,
                                 ry1 = y * scale;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r, 1);
                }
            }
        }
        QRcode_free(qr);
        
        //在二维码上画图片  
        if(!inImage.isNull())
        {
            qreal x = (size.width() >> 1) - (size.width() / 10);
            qreal y = (size.height() >> 1) - (size.height() / 10);
            qreal w = size.width() / 5;
            qreal h = size.height() / 5;
            LOG_MODEL_DEBUG("CQRCode", "x:%f, y:%f", x, y);
            int penw = 3;
            painter.setBrush(QBrush(QColor(255, 255, 255)));
            painter.drawRect(x - penw, y - penw, w + (penw << 1), h + (penw << 1));
            painter.setPen(QPen(Qt::NoPen));
            painter.setBrush(QBrush(Qt::NoBrush));
            QRectF rect(x, y, w, h);
            painter.drawImage(rect, inImage);
        }
    }
    else
    {
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0, 0, size.width(), size.height());
        LOG_MODEL_ERROR("CTool", "QR FAIL:%d", errno);
    }

    qr = nullptr;
    return image;
#endif //RABBITIM_USE_LIBQRENCODE

    return QImage();
}

int CQRCode::ProcessQImage(QImage image, QString &szText)
{
    QString szMessage;
#ifdef RABBITIM_USE_QZXING
    QZXing decoder;
    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE |
                       QZXing::DecoderFormat_DATA_MATRIX |
                       QZXing::DecoderFormat_UPC_E |
                       QZXing::DecoderFormat_UPC_A |
                       QZXing::DecoderFormat_EAN_8 |
                       QZXing::DecoderFormat_EAN_13 |
                       QZXing::DecoderFormat_CODE_128 |
                       QZXing::DecoderFormat_CODE_39 |
                       QZXing::DecoderFormat_ITF |
                       QZXing::DecoderFormat_Aztec);
    szMessage = decoder.decodeImage(image);
    if(szMessage.isEmpty())
    {
        LOG_MODEL_ERROR("CQRCode", "Scan image fail.");
        return 1;
    }
    LOG_MODEL_DEBUG("CQRCode", "Decode:%s", szMessage.toStdString().c_str());
#endif

    szText = szMessage;
    return ProcessMessage(szMessage);
}

int CQRCode::ProcessQImage(const std::shared_ptr<CVideoFrame> &frame, QString &szText)
{
    if(VIDEO_FORMAT_NONE == frame->m_VideoInfo.Format)
        return -1;
    std::shared_ptr<CVideoFrame> outFrame;
    if (VIDEO_FORMAT_RGB32 != frame->m_VideoInfo.Format)
    {
        CTool::ConvertFormat(frame, outFrame,
                             frame->m_VideoInfo.nWidth,
                             frame->m_VideoInfo.nHeight,
                             VIDEO_FORMAT_RGB32);
    }
    else
    {
        outFrame = frame;
    }
    QImage img((const uchar*)outFrame->GetData(),
               outFrame->m_VideoInfo.nWidth,
               outFrame->m_VideoInfo.nHeight,
               QImage::Format_RGB32);
    if(img.isNull())
    {
        LOG_MODEL_ERROR("CQRCode", "Image is null");
        return -1;
    }
    return ProcessQImage(img, szText);
}

int CQRCode::ProcessQRFile(const QString &szFile, QString &szText)
{
    QImage img(szFile);
    if(img.isNull())
    {
        LOG_MODEL_ERROR("CQRCode", "This isn't image file:%s",
                        szFile.toStdString().c_str());
        return 1;
    }
    return ProcessQImage(img, szText);
}

QImage CQRCode::QRcodeEncodeUserInfo(const QString &szInfo,
                                     const QImage &inImage)
{
    QString szData = RABBITIM_USERINFO + szInfo;
    return QRcodeEncodeString(szData, inImage);
}

QImage CQRCode::QRcodeEncodeUserId(const QString &szId, const QImage &inImage)
{
    QString szData = RABBITIM_ID + szId;
    return QRcodeEncodeString(szData, inImage);
}

/* > 0 ：
 * = 0 ：成功处理
 * < 0 : 错误
 */
int CQRCode::ProcessMessage(const QString &szMessage)
{
    QUrl url(szMessage);
    if("rabbitim" == url.scheme())
    {
        QString szId;
        if("id" == url.host()) //增加好友  
        {
            szId = szMessage.right(szMessage.size()
                                   - QString(RABBITIM_ID).size());
        } else if("userinfo" == url.host()) {
            int nId = szMessage.indexOf("Id:",
                                        szMessage.indexOf(RABBITIM_USERINFO),
                                        Qt::CaseInsensitive) + 3;
            int nRight = szMessage.indexOf(";", nId);
            szId = szMessage.mid(nId, nRight - nId);
        }
        LOG_MODEL_DEBUG("CQRCode", "ID:%s", szId.toStdString().c_str());
        if(szId.isEmpty())
            return -1;
        if(USER_INFO_LOCALE->GetInfo()->GetId() == szId)
        {
            LOG_MODEL_ERROR("CQRCode", "Roster[%s] is self.",
                            szId.toStdString().c_str());
            return 0;
        }
        QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szId);
        if(user.isNull())
        {
            QMessageBox msg(QMessageBox::Question, "",
                            tr("Do you add roster ") + szId + "?",
                            QMessageBox::Ok | QMessageBox::No,
                            NULL, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |Qt::WindowStaysOnTopHint);
            if(QMessageBox::Ok == msg.exec())
            {
                GET_CLIENT->RosterAdd(szId);
            }
        } else  {
            LOG_MODEL_ERROR("CQRCode", "Roster[%s] has exist.",
                            szId.toStdString().c_str());
        }
        return 0;
    } else {
        if(url.scheme() == "http"
                || url.scheme() == "https"
                || url.scheme() == "ftp"
                || url.scheme() == "mailto")
        {
            QMessageBox msg(QMessageBox::Question, "",
                            tr("Do open ") + szMessage + "?",
                            QMessageBox::Ok | QMessageBox::No,
                            NULL, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |Qt::WindowStaysOnTopHint);
            if(QMessageBox::Ok == msg.exec())
                if(QDesktopServices::openUrl(url))
                    return 0;
            else
                return 0;
        }
    }
    return -4;
}
