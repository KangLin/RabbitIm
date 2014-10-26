#ifndef CCALLACTION_H
#define CCALLACTION_H

#include "Message/ChatActions/ChatAction.h"
#include "CallObject.h"
#include <QSharedPointer>

class CCallAction : public CChatAction
{
public:
    CCallAction(QSharedPointer<CCallObject> call, const QString &author, const QTime &date, const bool &me);
    virtual ~CCallAction();

    virtual QString getMessage();

private:
    QString getDescriptionConnectingState();
    QString getDescriptionActiveState();
    QString getDescriptionDisconnectingState();
    QString getDescriptionFinishedState();
    QString drawButton(const QString &szHref, const QString &szText, const QString &szIcon = QString());
    QString drawAccept(QString szHref);
    QString drawCancel(QString szHref);

private:
    QSharedPointer<CCallObject> m_Call;
};

#endif // CCALLACTION_H
