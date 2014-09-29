#include "MessageAction.h"
#include "../SmileyPack.h"
#include "Global/Global.h"

CMessageAction::CMessageAction(const QString &author, const QString &message, const QString &date, const bool &me) :
    CChatAction(me, author, date),
    message(message)
{
}

void CMessageAction::setup(QTextCursor cursor, QTextEdit *)
{
    // When this function is called, we're supposed to only update ourselve when needed
    // Nobody should ask us to do anything with our content, we're on our own
    // Except we never udpate on our own, so we can safely free our resources

    (void) cursor;
    message.clear();
    message.squeeze();
    szId.clear();
    szId.squeeze();
    date.clear();
    date.squeeze();
}

QString CMessageAction::getMessage()
{
    QString message_ = CSmileyPack::getInstance().smileyfied(toHtmlChars(message));

    // detect urls
    QRegExp exp("(www\\.|http[s]?:\\/\\/|ftp:\\/\\/)\\S+");
    int offset = 0;
    while ((offset = exp.indexIn(message_, offset)) != -1)
    {
        QString url = exp.cap();

        // add scheme if not specified
        if (exp.cap(1) == "www.")
            url.prepend("http://");

        QString htmledUrl = QString("<a color=#ff0000 href=\"%1\">%1</a>").arg(url);
        message_.replace(offset, exp.cap().length(), htmledUrl);

        offset += htmledUrl.length();
    }

    // detect text quotes
    QStringList messageLines = message_.split("\n");
    message_ = "";
    for (QString& s : messageLines)
    {
        if (QRegExp("^[ ]*&gt;.*").exactMatch(s))
            message_ += "<span style=color:#6bc260;>" + s.right(s.length()-4) + "</span><br/>";
        else
            message_ += s + "<br/>";
    }
    message_ = message_.left(message_.length()-4);

    return message;
}

QString CMessageAction::getContent()
{
    QString msg;
   msg += "<img src='";
   msg += CGlobal::Instance()->GetFileUserAvatar(szId) + "' width='16' height='16'>";
   msg += "<font color='";
   if(!this->isMe)
        msg += CGlobal::Instance()->GetRosterColor().name();
   else
        msg += CGlobal::Instance()->GetUserColor().name();
   msg += "'>[";
   msg += QTime::currentTime().toString() + "]"
           + GLOBAL_USER->GetUserInfoRoster(szId)->GetInfo()->GetShowName()
           + ":</font><br /><font color='";
   if(!isMe)
        msg += CGlobal::Instance()->GetRosterMessageColor().name();
   else
        msg += CGlobal::Instance()->GetUserMessageColor().name();
   msg += "'>";
   msg += "</font>";
   msg += getMessage();

   return msg;
}
