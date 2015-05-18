#ifndef CALLOBJECT_H
#define CALLOBJECT_H

#include <QObject>
#include <QIODevice>
#include <QTime>
#include <QSound>

/**
  * @defgroup RABBITIM_INTERFACE_CALLOBJECT 呼叫接口类模块  
  * @ingroup RABBITIM_INTERFACE_MANGECALL
  * @brief 呼叫接口类模块  
  */

/**
 * @ingroup RABBITIM_INTERFACE_CALLOBJECT RABBITIM_INTERFACE
 * @brief 呼叫接口类
 */
class CCallObject : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 
     *
     * @param bVideo:视频呼叫  
     * @param parent
     */
    explicit CCallObject(bool bVideo = false, QObject *parent = 0);
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

public slots:
    virtual int Accept();//=0
    virtual int Cancel();//=0

public:
    virtual QString GetId();
    virtual State GetState(); //=0
    virtual Direction GetDirection();

    virtual bool IsVideo();//是否是视频呼叫  
protected:
    virtual int SetId(const QString szId);
    virtual int SetDirection(Direction d);
    virtual void PlayCallSound();
    virtual void StopCallSound();

private slots:

signals:
    void sigUpdate();
    void sigFinished(CCallObject *pCall);

private:
    QString m_szId;
    Direction m_Direction;//呼叫方向  
    QSound* m_pSound;

protected:
    bool m_bVideo;
};

#endif // CALLOBJECT_H
