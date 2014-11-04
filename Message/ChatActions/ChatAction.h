#ifndef CHATACTION_H
#define CHATACTION_H

#include <QString>
#include <QTextCursor>
#include <QDate>
#include <QTextEdit>
#include <QObject>

class CChatAction : public QObject
{
    Q_OBJECT
public:
    CChatAction(const bool &me, const QString &author, const QTime &date);
    virtual ~CChatAction();
    virtual void setup(QTextCursor cursor, QTextEdit* textEdit); ///< Call once, and then you MUST let the object update itself

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
    QString drawButton(const QString &szHref, const QString &szText, const QString &szIcon = QString());
    QString drawAccept(QString szHref);
    QString drawCancel(QString szHref);
    
protected:
    bool m_isMe;
    QString m_szId;
    QTime m_Time;
    QTextCursor m_Cursor;
    QTextEdit* m_pEdit;
};

#endif // CHATACTION_H
