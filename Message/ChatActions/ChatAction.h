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

protected:
    bool m_isMe;
    QString m_szId;
    QDate m_Date;
};

#endif // CHATACTION_H
