#ifndef CALLOBJECT_H
#define CALLOBJECT_H

#include <QObject>
#include <QIODevice>
#include <QTime>

class CCallObject : public QObject
{
    Q_OBJECT
public:
    explicit CCallObject(QObject *parent = 0);

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

    virtual int Accept();
    virtual int Cancel();

    virtual QString GetId();
    virtual int GetTotalTime();//得到通话时间  
    virtual State GetState();

protected:
    virtual int SetId(const QString szId);

signals:
    /// \brief This signal is emitted when a call is connected.
    ///
    /// Once this signal is emitted, you can connect a QAudioOutput and
    /// QAudioInput to the call. You can determine the appropriate clockrate
    /// and the number of channels by calling payloadType().
    void sigConnected();

    /// \brief This signal is emitted when a call is finished.
    ///
    /// Note: Do not delete the call in the slot connected to this signal,
    /// instead use deleteLater().
    void sigFinished();

    /// \brief This signal is emitted when the remote party is ringing.
    void sigRinging();

    /// \brief This signal is emitted when the call state changes.
    void sigStateChanged(State state);

    /// \brief This signal is emitted when the audio channel changes.
    void audioModeChanged(QIODevice::OpenMode mode);

    /// \brief This signal is emitted when the video channel changes.
    void videoModeChanged(QIODevice::OpenMode mode);

private:
    QString m_szId;

protected:
    QTime m_tmStart;  //呼叫开始时间  
    int m_nTotal;  //呼叫总时间  
};

#endif // CALLOBJECT_H
