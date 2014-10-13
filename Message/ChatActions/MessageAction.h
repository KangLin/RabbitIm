#ifndef MESSAGEACTION_H
#define MESSAGEACTION_H

#include "ChatAction.h"

class CMessageAction : public CChatAction
{
public:
    CMessageAction(const QString &author, const QString &message, const QDate &date, const bool &me);
    virtual ~CMessageAction(){;}
    virtual QString getMessage();
    //virtual QString getContent();
    virtual void setup(QTextCursor cursor, QTextEdit*) override;

private:
    QString m_szMessage;
};

#endif // MESSAGEACTION_H
