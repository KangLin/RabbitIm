#ifndef CCALLACTION_H
#define CCALLACTION_H

#include "Message/ChatActions/ChatAction.h"
#include "CallObject.h"
#include <QSharedPointer>
#include <QTimer>
#include "Global/GlobalDir.h"

/**
 * @brief 呼叫动作类  
 * @see CCallObject
 */
class RABBITIM_SHARED_LIBRARY CCallAction : public CChatAction
{
    Q_OBJECT
public:
    explicit CCallAction(QSharedPointer<CCallObject> call,
                         const QString &szId,
                         const QTime &date,
                         const bool &me);
    virtual ~CCallAction();

    virtual QString getMessage();

private slots:
    void slotUpdateHtml();

protected:
    virtual QString getDescriptionCallState();
    virtual QString getDescriptionConnectingState();
    virtual QString getDescriptionActiveState();
    virtual QString getDescriptionFinishedState();

    virtual QString getPrompt();

private:
    QSharedPointer<CCallObject> m_Call;
    QTime m_tmStart;
    QTimer m_Timer;
};

#endif // CCALLACTION_H
