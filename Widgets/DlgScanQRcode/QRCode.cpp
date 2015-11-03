#include "QRCode.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QPainter>
#include "MainWindow.h"

#ifdef RABBITIM_USE_LIBQRENCODE
#include "qrencode.h"
#endif

#ifdef RABBITIM_USE_QZXING
    #include "qzxing.h"
#endif

#define RABBITIM_ID "rabbitim://id/"
#define RABBITIM_USERINFO "rabbitim://userinfo/"

CQRCode::CQRCode() : QObject()
{    
}

//http://stackoverflow.com/questions/21400254/how-to-draw-a-qr-code-with-qt-in-native-c-c
QImage CQRCode::QRcodeEncodeString(const QString &szData, const QSize &size)
{
    QImage image(size, QImage::Format_RGB32);
    QPainter painter(&image);
    
#ifdef RABBITIM_USE_LIBQRENCODE
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
    }
    else
    {
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0, 0, size.width(), size.height());
        LOG_MODEL_ERROR("CTool", "QR FAIL:%d", errno);
    }

    qr = nullptr;
#endif //RABBITIM_USE_LIBQRENCODE
    return image;
}

QString CQRCode::ProcessQImage(QImage image)
{
#ifdef RABBITIM_USE_QZXING
    QZXing decoder;
    QString szMessage = decoder.decodeImage(image);
    if(szMessage.isEmpty())
    {
        LOG_MODEL_ERROR("CDlgScanQRcode", "Scan image fail.");
        return "";
    }
    LOG_MODEL_DEBUG("CQRCode", "Decode:%s", szMessage.toStdString().c_str());
#endif

    if(!ProcessMessage(szMessage))
        return "";

    return szMessage;
}

QString CQRCode::ProcessQRFile(const QString &szFile)
{
    QImage img(szFile);
    if(img.isNull())
    {
        LOG_MODEL_ERROR("CDlgScanQRcode", "This isn't image file:%s",
                        szFile.toStdString().c_str());
        return "";
    }
    return ProcessQImage(img);
}

QImage CQRCode::QRcodeEncodeUserInfo(const QString &szInfo)
{
    QString szData = RABBITIM_USERINFO + szInfo;
    return QRcodeEncodeString(szData);
}

QImage CQRCode::QRcodeEncodeUserId(const QString &szId)
{
    QString szData = RABBITIM_ID + szId;
    return QRcodeEncodeString(szData);
}

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
        LOG_MODEL_DEBUG("CDlgScanQRcode", "ID:%s", szId.toStdString().c_str());
        if(szId.isEmpty())
            return -1;
        if(USER_INFO_LOCALE->GetInfo()->GetId() == szId)
        {
            LOG_MODEL_ERROR("CDlgScanQRcode", "Roster[%s] is self.",
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
                return 0;
            }
        } else  {
            LOG_MODEL_ERROR("CDlgScanQRcode", "Roster[%s] has exist.",
                            szId.toStdString().c_str());
            return 0;
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
        }
    }
    return -4;
}
