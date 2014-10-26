#include "CallAction.h"
#include "Global/Global.h"

CCallAction::CCallAction(QSharedPointer<CCallObject> call, const QString &author, const QTime &date, const bool &me) :
    CChatAction(me, author, date)
{
    m_Call = call;
}

CCallAction::~CCallAction()
{
}

QString CCallAction::getMessage()
{
    QString szMsg;
    szMsg = "<table>";
    switch(m_Call->GetState())
    {
    case CCallObject::ConnectingState:
        szMsg += getDescriptionConnectingState();
        break;
    case CCallObject::ActiveState:
        szMsg += getDescriptionActiveState();
        break;
    case CCallObject::DisconnectingState:
        szMsg += getDescriptionDisconnectingState();
        break;
    case CCallObject::FinishedState:
        szMsg += getDescriptionFinishedState();
        break;
    default:
        break;
    };
    szMsg += "</table>";
    return szMsg;
}

QString CCallAction::drawButton(const QString &szHref, const QString &szText, const QString &szIcon)
{
    QString szMsg;
    szMsg = "<td align='center'><a href='" + szHref + "'>";
    if(!szIcon.isEmpty())
        szMsg += QImage2Html(QImage(szIcon, "png"), 16, 16);
    szMsg += szText + "</a></td>";
    return szMsg;
}

QString CCallAction::drawAccept(QString szHref)
{
    return drawButton(szHref, tr("Accpet"), ":/icon/Accept");
}

QString CCallAction::drawCancel(QString szHref)
{
    return drawButton(szHref, tr("Cancel"), ":/icon/Cancel");
}

QString CCallAction::getDescriptionConnectingState()
{
    QString szMsg;
    szMsg = "<tr>";
    if(m_isMe)
        szMsg += "<td align='center'>" + tr("Be launching a video call");
    else
        szMsg += "<td colspan='2' align='center'>" + tr("Be receiving a video call");
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    if(!m_isMe)
        szMsg += drawAccept("rabbitim://CallVideo?command=accept");
    szMsg += drawCancel("rabbitim://CallVideo?command=cancel");
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionActiveState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td align='center'>" + tr("Be talking ...");
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawCancel("rabbitim://CallVideo?command=cancel");
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionDisconnectingState()
{
    QString szMsg;
    
    return szMsg;
}

QString CCallAction::getDescriptionFinishedState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td align='center'>" + tr("video call over");
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawButton("rabbitim://CallVideo?command=call", tr("Call"));
    szMsg += "</tr>";
    return szMsg;
}
