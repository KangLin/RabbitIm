#ifndef CALLOBJECT_H
#define CALLOBJECT_H

#include <QObject>
#include <QIODevice>
#include <QTime>
#include <QSound>

class CCallObject : public QObject
{
    Q_OBJECT
public:
    explicit CCallObject(QObject *parent = 0);
    virtual ~CCallObject();

    enum Direction
    {
        IncomingDirection, ///< The call is incoming.
        OutgoingDirection, ///< The call is outgoing.
    };

    /// This enum is used to describe the state of a call.
    enum State
    {
        ConnectingState = 0,    ///< The call is being connected.
        ActiveState = 1,        ///< The call is active.
        DisconnectingState = 2, ///< The call is being disconnected.
        FinishedState = 3,      ///< The call is finished.
    };

    virtual int Accept();//=0
    virtual int Cancel();//=0

    virtual QString GetId();
    virtual State GetState(); //=0
    virtual Direction GetDirection();

protected:
    virtual int SetId(const QString szId);
    virtual int SetDirection(Direction d);
    virtual void PlayCallSound();
    virtual void StopCallSound();

private slots:

signals:
    void sigUpdate();
    void sigFinished(QSharedPointer<CCallObject> call);

private:
    QString m_szId;
    Direction m_Direction;//呼叫方向  
    QSharedPointer <QSound> m_Sound;
};

#endif // CALLOBJECT_H
