#include "CallAction.h"
#include "Global/Global.h"
#include <QTextEdit>
#include <QScrollBar>

CCallAction::CCallAction(QSharedPointer<CCallObject> call,
                         const QString &szId,
                         const QTime &date,
                         const bool &me)
    : CChatAction(me, szId, date)
{
    m_Call = call;
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(m_szId);
    if(!roster.isNull())
    {
        if(roster->GetInfo()->GetIsMonitor()
                && CGlobal::Instance()->GetIsMonitor()
                && !m_isMe)
            m_tmStart = date;
    }
    bool check = connect(m_Call.data(), SIGNAL(sigUpdate()),this, SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
    check = connect(&m_Timer, SIGNAL(timeout()),
                    SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
}

CCallAction::~CCallAction()
{
    if(m_Timer.isActive())
        m_Timer.stop();
    LOG_MODEL_DEBUG("CCallAction", "CCallAction::~CCallAction");
}

QString CCallAction::getMessage()
{
    QString szMsg;
    szMsg = "<table>";
    //LOG_MODEL_DEBUG("CCallAction", "state:%d", m_Call->GetState());
    if(m_Call.isNull())
        return QString();
    switch(m_Call->GetState())
    {
    case CCallObject::CallState:
        szMsg += getDescriptionCallState();
        break;
    case CCallObject::ConnectingState:
        szMsg += getDescriptionConnectingState();
        break;
    case CCallObject::ActiveState:
        if(!m_Timer.isActive())
        {
            QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(m_szId);
            if(!roster.isNull())
            {
                if(!(roster->GetInfo()->GetIsMonitor() 
                        && CGlobal::Instance()->GetIsMonitor() && !m_isMe))
                {
                    m_tmStart = QTime::currentTime();
                }
            }
            else
            {
                m_tmStart = QTime::currentTime();
            }
            m_Timer.start(1000);
        }
        szMsg += getDescriptionActiveState();
        break;
    case CCallObject::DisconnectingState:
        m_Timer.stop();
        szMsg += getDescriptionDisconnectingState();
        break;
    case CCallObject::FinishedState:
        m_Timer.stop();
        szMsg += getDescriptionFinishedState();
        break;
    default:
        break;
    };
    szMsg += "</table>";
    return szMsg;
}

QString CCallAction::getDescriptionCallState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td colspan='2' align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    if(!m_isMe)
        szMsg += drawAccept("rabbitim://call?command=accept");
    szMsg += drawCancel("rabbitim://call?command=cancel");
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionConnectingState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td colspan='2' align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawCancel("rabbitim://call?command=cancel");
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionActiveState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawCancel("rabbitim://call?command=cancel");
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionDisconnectingState()
{
    return getDescriptionFinishedState();
}

QString CCallAction::getDescriptionFinishedState()
{
    QString szMsg;
    QString szCommand;
    if(m_Call->IsVideo())
        szCommand = "rabbitim://call?command=call&video=true";
    else
        szCommand = "rabbitim://call?command=call&video=false";

    szMsg = "<tr>";
    szMsg += "<td align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawButton(szCommand, tr("Call"));
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getPrompt()
{
    QString szMsg;
    switch(m_Call->GetState())
    {
    case CCallObject::CallState:
        if(m_Call->IsVideo())
            szMsg = tr("Being a video ringing ......");
        else
            szMsg = tr("Being a ringing ......");
        break;
    case CCallObject::ConnectingState:
        szMsg = tr("Be connecting ......");
        break;
    case CCallObject::ActiveState:
        if(m_Call->IsVideo())
            szMsg = tr("Be a video talking ...... ; ");
        else
            szMsg = tr("Be talking ...... ; ");
        szMsg += tr("Talk time: ") + QString::number(m_tmStart.secsTo(QTime::currentTime())) + tr("s");
        break;
    case CCallObject::DisconnectingState:
        if(m_Call->IsVideo())
            szMsg = tr("Video talk is disconnected");
        else
            szMsg = tr("Talk is disconnected");
        szMsg += tr("Talk time: ") + QString::number(m_tmStart.secsTo(QTime::currentTime())) + tr("s");
        break;
    case CCallObject::FinishedState:
        if(m_Call->IsVideo())
            szMsg = tr("Video talk over.");
        else
            szMsg = tr("Talk over.");
        szMsg += tr("Talk time: ") + QString::number(m_tmStart.secsTo(QTime::currentTime())) + tr("s");
        break;
    default:
        break;
    };
    return szMsg;
}

void CCallAction::slotUpdateHtml()
{
    if (m_Cursor.isNull() || !m_pEdit)
        return;

    // save old slider value
    int vSliderVal = m_pEdit->verticalScrollBar()->value();

    // update content
    int pos = m_Cursor.selectionStart();
    m_Cursor.removeSelectedText();
    m_Cursor.setKeepPositionOnInsert(false);
    m_Cursor.insertHtml(getContent());
    m_Cursor.setKeepPositionOnInsert(true);
    int end = m_Cursor.position();
    m_Cursor.setPosition(pos);
    m_Cursor.setPosition(end, QTextCursor::KeepAnchor);

    // restore old slider value
    m_pEdit->verticalScrollBar()->setValue(vSliderVal);

    // Free our ressources if we'll never need to update again
    if(m_Call->GetState() == CCallObject::FinishedState)
    {
        m_Cursor = QTextCursor();
        m_Call.clear();
    }
}
