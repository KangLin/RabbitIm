#include "ChatAction.h"
#include <QStringList>
#include <QBuffer>
#include "Global/Global.h"
#include <initializer_list>

CChatAction::CChatAction(const bool &me, const QString &szId, const QTime &date)
 : QObject(0), m_isMe(me), m_szId(szId), m_Time(date)
{
    m_pEdit = NULL;
}

CChatAction::~CChatAction()
{
}

QString CChatAction::toHtmlChars(const QString &str)
{
    static QList<QPair<QString, QString> > replaceList = {
        {"\"", "&quot;"}, 
        {"&","&amp;"}, 
        {">","&gt;"}, 
        {"<","&lt;"}
    };
    QString res = str;

    for (auto &it : replaceList)
        res = res.replace(it.first,it.second);

    return res;
}

QString CChatAction::QImage2base64(const QImage &img)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG"); // writes image into ba in PNG format
    return ba.toBase64();
}

QString CChatAction::QImage2Html(const QImage &img, int nWidth, int nHeight)
{
    QString szHtml = "<img ";
    if(nWidth !=0 && 0 != nHeight)
    {
        szHtml += " width='" + QString::number(nWidth)
            + "' height='" + QString::number(nHeight) + "' ";
    }
    szHtml  += " src=\"data:image/png;base64," + QImage2base64(img) + "\">";
    return szHtml;
}

QString CChatAction::drawButton(const QString &szHref, const QString &szText, const QString &szIcon)
{
    QString szMsg;
    szMsg = "<td align='center'><a href='" + szHref + "'>";
    if(!szIcon.isEmpty())
        szMsg += QImage2Html(QImage(szIcon, "png"), 16, 16);
    szMsg += szText + "</a></td>";
    return szMsg;
}

QString CChatAction::drawAccept(QString szHref)
{
    return drawButton(szHref, tr("Accpet"), ":/icon/Accept");
}

QString CChatAction::drawCancel(QString szHref)
{
    return drawButton(szHref, tr("Cancel"), ":/icon/Cancel");
}

QString CChatAction::getName()
{
    QString name;
    if (m_isMe)
    {
        name = GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName();
        return QString("<div class=name_me>" + toHtmlChars(name) + "</div>");
    }

    name = GLOBAL_USER->GetUserInfoRoster(m_szId)->GetInfo()->GetShowName();
    return QString("<div class=name>" + toHtmlChars(name) + "</div>");
}

QString CChatAction::getDate()
{
    QString res = m_Time.toString();
    return res;
}

QString CChatAction::getContent()
{
   QString msg;
   QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(m_szId);
   if(roster.isNull())
   {
#ifdef DEBUG
       Q_ASSERT(false);
#endif
       return "";
   }

   msg = "<table width='100%'><tr><td align='";
   if(m_isMe)
        msg += "right'>";
   else
       msg += "left'>";

   if(!m_isMe)
        msg += "<a href='rabbitim://userinfo'>";
   msg += "<img src='";
   if(m_isMe)
        msg += CGlobalDir::Instance()->GetFileUserAvatar(GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetId());
   else
        msg += CGlobalDir::Instance()->GetFileUserAvatar(m_szId);
   msg += "' width='" + QString::number(RABBITIM_ICON_SIZE) + "' height='" + QString::number(RABBITIM_ICON_SIZE) + "' />";
   msg += "<font color='";
   if(!m_isMe)
        msg += CGlobal::Instance()->GetRosterColor().name();
   else
        msg += CGlobal::Instance()->GetUserColor().name();
   msg += "'>[";
   msg += m_Time.toString() + "]";
   if(!m_isMe)
        msg += roster->GetInfo()->GetShowName();
   else
        msg += GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName();
   msg += ":";
   msg += "</font>";
   if(!m_isMe)
        msg += "</a>";

   msg += "</td></tr><tr><td align='";
   if(m_isMe)
        msg += "right'>";
   else
       msg += "left'>";
   msg += getMessage();
   msg += "</td></tr></table>";
   //LOG_MODEL_DEBUG("CChatAction", qPrintable(msg));
  return msg;
}

void CChatAction::setup(QTextCursor cursor, QTextEdit *textEdit)
{
    m_Cursor = cursor;
    m_Cursor.setKeepPositionOnInsert(true);
    int end=m_Cursor.selectionEnd();
    m_Cursor.setPosition(m_Cursor.position());
    m_Cursor.setPosition(end, QTextCursor::KeepAnchor);

    m_pEdit = textEdit;
}
