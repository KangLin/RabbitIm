#ifndef CHATACTION_H
#define CHATACTION_H

#include <QString>
#include <QTextCursor>
#include <QDate>
#include <QTextEdit>
#include <QObject>
#include "Global/GlobalDir.h"

/**
 * @defgroup RABBITIM_INTERFACE_CHATACTION 动作接口类模块  
 * @ingroup RABBITIM_INTERFACE_MANAGEMESSAGE
 * @brief 动作接口类模块。消息界面调用此接口得到相应动作的描述  
 */

/**
 * @class CChatAction
 * @ingroup RABBITIM_INTERFACE_CHATACTION RABBITIM_INTERFACE
 * @brief 动作接口类。消息界面调用此接口得到相应动作的描述  
 * @see CManageMessage
 */
class RABBITIM_SHARED_LIBRARY CChatAction : public QObject
{
    Q_OBJECT
public:
    CChatAction(const bool &me, const QString &szId, const QTime &date);
    virtual ~CChatAction();
    virtual void setup(QTextCursor cursor, QTextEdit* textEdit); ///< Call once, and then you MUST let the object update itself

    virtual QString getName();
    /**
     * @brief 消息界面调用此接口得到相应动作的描述。具体功能由派生类实现。  
     * @return 
     */
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
