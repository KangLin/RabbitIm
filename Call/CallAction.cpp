#include "CallAction.h"
#include "Global/Global.h"
#include <QTextEdit>
#include <QScrollBar>

CCallAction::CCallAction(QSharedPointer<CCallObject> call, const QString &author, const QTime &date, const bool &me) 
    : CChatAction(me, author, date)
    , m_Call(call)
{
    bool check = connect(m_Call.data(), SIGNAL(sigUpdate()),this, SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
    check = connect(&m_Timer, SIGNAL(timeout()),
                    SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
}

CCallAction::~CCallAction()
{
}

QString CCallAction::getMessage()
{
    QString szMsg;
    szMsg = "<table>";
    //LOG_MODEL_DEBUG("CCallAction", "state:%d", m_Call->GetState());
    switch(m_Call->GetState())
    {
    case CCallObject::ConnectingState:
        szMsg += getDescriptionConnectingState();
        break;
    case CCallObject::ActiveState:
        if(!m_Timer.isActive())
        {
            m_tmStart = QTime::currentTime();
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

QString CCallAction::getDescriptionConnectingState()
{
    QString szMsg;
    szMsg = "<tr>";
    if(m_isMe)
        szMsg += "<td align='center'>" + tr("Be launching a call");
    else
        szMsg += "<td colspan='2' align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    if(!m_isMe)
        szMsg += drawAccept("rabbitim://call?command=accept");
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
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawButton("rabbitim://call?command=call", tr("Call"));
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getDescriptionFinishedState()
{
    QString szMsg;
    szMsg = "<tr>";
    szMsg += "<td align='center'>" + getPrompt();
    szMsg += "</td></tr>";
    szMsg += "<tr>";
    szMsg += drawButton("rabbitim://call?command=call", tr("Call"));
    szMsg += "</tr>";
    return szMsg;
}

QString CCallAction::getPrompt()
{
    QString szMsg;
    switch(m_Call->GetState())
    {
    case CCallObject::ConnectingState:
        szMsg = tr("Be receiving a call");
        break;
    case CCallObject::ActiveState:
        szMsg = tr("Be talking ..., talk time:") + QString::number(m_tmStart.secsTo(QTime::currentTime()));
        break;
    case CCallObject::DisconnectingState:
        szMsg = tr("talk is disconnected");
        break;
    case CCallObject::FinishedState:
        szMsg = tr("talk over: talk time:") + QString::number(m_tmStart.secsTo(QTime::currentTime()));
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
    }
}
