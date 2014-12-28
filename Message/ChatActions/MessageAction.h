#ifndef MESSAGEACTION_H
#define MESSAGEACTION_H

#include "ChatAction.h"

class CMessageAction : public CChatAction
{
public:
    CMessageAction(const QString &szId, const QString &message, const QTime &date, const bool &me);
    virtual ~CMessageAction(){;}
    virtual QString getMessage();
    virtual void setup(QTextCursor cursor, QTextEdit*);

private:
    QString m_szMessage;
};

#endif // MESSAGEACTION_H
