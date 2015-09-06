#include "ChatActionGroupChat.h"
#include "GroupChat.h"
#include "ManageGroupChat.h"
#include "Global/Global.h"
#include <QFile>

CChatActionGroupChat::CChatActionGroupChat(const QString &szId, const QString &message, const QString &szSendId, const QTime &time, const bool &me) :
    CMessageAction(szId, message, time, me),
    m_szSendId(szSendId)
{
}

QString CChatActionGroupChat::getContent()
{
   QString msg;
   QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(m_szId);
   if(gc.isNull())
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
   {
       QString szFile = CGlobalDir::Instance()->GetFileUserAvatar(gc->ParticipantId(m_szSendId));
       QFile file(szFile);
       if(!file.exists())
           szFile = ":/icon/AppIcon";
       msg += szFile;
   }
   msg += "' width='16' height='16' />";
   msg += "<font color='";
   if(!m_isMe)
        msg += CGlobal::Instance()->GetRosterColor().name();
   else
        msg += CGlobal::Instance()->GetUserColor().name();
   msg += "'>[";
   msg += m_Time.toString() + "]";
   if(!m_isMe)
        msg += gc->ParticipantNick(m_szSendId);
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
