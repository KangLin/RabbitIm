#ifndef CCALLACTION_H
#define CCALLACTION_H

#include "Message/ChatActions/ChatAction.h"
#include "CallObject.h"
#include <QSharedPointer>
#include <QTimer>

class CCallAction : public CChatAction
{
    Q_OBJECT
public:
    explicit CCallAction(QSharedPointer<CCallObject> call, const QString &author, const QTime &date, const bool &me);
    ~CCallAction();

    virtual QString getMessage();

private slots:
    void slotUpdateHtml();

protected:
    virtual QString getDescriptionConnectingState();
    virtual QString getDescriptionActiveState();
    virtual QString getDescriptionDisconnectingState();
    virtual QString getDescriptionFinishedState();

    virtual QString getPrompt();

private:
    QSharedPointer<CCallObject> m_Call;
    QTime m_tmStart;
    QTimer m_Timer;
};

#endif // CCALLACTION_H
