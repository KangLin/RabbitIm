#include "MessageAction.h"
#include "../SmileyPack.h"
#include "Global/Global.h"

CMessageAction::CMessageAction(const QString &author, const QString &message, const QTime &date, const bool &me) :
    CChatAction(me, author, date),
    m_szMessage(message)
{
}

void CMessageAction::setup(QTextCursor cursor, QTextEdit *)
{
    // When this function is called, we're supposed to only update ourselve when needed
    // Nobody should ask us to do anything with our content, we're on our own
    // Except we never udpate on our own, so we can safely free our resources

    (void) cursor;
    m_szMessage.clear();
    m_szMessage.squeeze();
    m_szId.clear();
    m_szId.squeeze();
}

QString CMessageAction::getMessage()
{
    QString message_ = CSmileyPack::getInstance().smileyfied(toHtmlChars(m_szMessage));

    // detect urls
    QRegExp exp("(www\\.|http[s]?:\\/\\/|ftp:\\/\\/)\\S+");
    int offset = 0;
    while ((offset = exp.indexIn(message_, offset)) != -1)
    {
        QString url = exp.cap();

        // add scheme if not specified
        if (exp.cap(1) == "www.")
            url.prepend("http://");

        QString htmledUrl = QString("<a href=\"%1\">%1</a>").arg(url);
        message_.replace(offset, exp.cap().length(), htmledUrl);

        offset += htmledUrl.length();
    }

    message_ = message_.replace(QString("\n"), QString("<br/>"));

    QString msg;
    msg = "<div> <font='";
    if(m_isMe)
        msg += CGlobal::Instance()->GetUserMessageColor().name();
    else
         msg += CGlobal::Instance()->GetRosterMessageColor().name();
    msg+= "'>";
    msg += message_ + "</font></div>";
    return msg;
}

/*QString CMessageAction::getContent()
{
   QString msg;

   msg = "<div>";
   msg += "<div>";
   if(!this->isMe)
        msg += "<a href='rabbitim://userinfo'>";
   msg += "<img src='";
   msg += CGlobal::Instance()->GetFileUserAvatar(szId) + "' width='16' height='16' />";
   msg += "<font color='";
   if(!this->isMe)
        msg += CGlobal::Instance()->GetRosterColor().name();
   else
        msg += CGlobal::Instance()->GetUserColor().name();
   msg += "'>[";
   msg += QTime::currentTime().toString() + "]";
   msg += GLOBAL_USER->GetUserInfoRoster(szId)->GetInfo()->GetShowName() + ":";
   msg += "</font>";
   if(!this->isMe)
        msg += "</a>";
   msg += "</div>";
   msg += getMessage() + "</div>";
   LOG_MODEL_DEBUG("CMessageAction", qPrintable(msg));
   return msg;
}
*/
