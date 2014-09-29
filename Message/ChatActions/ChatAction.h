#ifndef CHATACTION_H
#define CHATACTION_H

#include <QString>
#include <QTextCursor>

class FileTransferInstance;
class QTextEdit;

class CChatAction : public QObject
{
public:
    CChatAction(const bool &me, const QString &author, const QString &date) : isMe(me), szId(author), date(date) {;}
    virtual ~CChatAction(){;}
    virtual void setup(QTextCursor cursor, QTextEdit* textEdit) = 0; ///< Call once, and then you MUST let the object update itself

    virtual QString getName();
    virtual QString getMessage() = 0;
    virtual QString getDate();

    virtual QString getContent() = 0;

protected:
    QString toHtmlChars(const QString &str);
    QString QImage2base64(const QImage &img);

protected:
    bool isMe;
    QString szId, date;
};

#endif // CHATACTION_H
