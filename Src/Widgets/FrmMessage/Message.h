#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QTime>

class CMessage
{
public:
    CMessage();
    CMessage(QString &fromBareJid, QString &toBareJid, QString &message);
    virtual ~CMessage();

    QString GetMessage();
    QString GetFromBareJid();
    QString GetToBareJid();
    QTime GetTime();

private:
    QString m_Message;
    QString m_FromBareJid;
    QString m_ToBareJid;
    QTime m_Time;

};

#endif // MESSAGE_H
