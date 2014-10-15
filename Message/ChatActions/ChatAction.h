#ifndef CHATACTION_H
#define CHATACTION_H

#include <QString>
#include <QTextCursor>
#include <QDate>

class FileTransferInstance;
class QTextEdit;

class CChatAction : public QObject
{
public:
    CChatAction(const bool &me, const QString &author, const QDate &date) : m_isMe(me), m_szId(author), m_Date(date) {;}
    virtual ~CChatAction(){;}
    virtual void setup(QTextCursor cursor, QTextEdit* textEdit) = 0; ///< Call once, and then you MUST let the object update itself

    virtual QString getName();
    virtual QString getMessage() = 0;
    virtual QString getDate();

    virtual QString getContent();

protected:
    QString toHtmlChars(const QString &str);
    QString QImage2base64(const QImage &img);
    /**
     * @brief 
     *
     * @param img
     * @param nWidth:为0时，为图片原始宽度  
     * @param nHeight：为0时，为图片原始高度  
     * @return QString
     */
    QString QImage2Html(const QImage&img, int nWidth = 0, int nHeight = 0);

protected:
    bool m_isMe;
    QString m_szId;
    QDate m_Date;
};

#endif // CHATACTION_H
