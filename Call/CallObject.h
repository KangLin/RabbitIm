#ifndef CALLOBJECT_H
#define CALLOBJECT_H

#include <QObject>
#include <QIODevice>
#include <QTime>
#include <QSound>
#include <QVideoFrame>
#include "Global/GlobalDir.h"

class CFrmVideo;

/**
  * @defgroup RABBITIM_INTERFACE_CALLOBJECT 呼叫接口类模块  
  * @ingroup RABBITIM_INTERFACE_MANGECALL  
  * @brief 呼叫接口类模块  
  */

/**
 * @ingroup RABBITIM_INTERFACE_CALLOBJECT RABBITIM_INTERFACE
 * @brief 呼叫接口类  
 * 在其派生类中实现信令协议、实现音视频数据的发送接收  
 * 
 */
class RABBITIM_SHARED_LIBRARY CCallObject : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造呼叫对象  
     *
     * @param bVideo:是否包含视频  
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
        CallState = -1,         ///< 呼叫状态  
        ConnectingState = 0,    ///< 建立连接状态  
        ActiveState = 1,        ///< 通话状态  
        DisconnectingState = 2, ///< 连接断开状态  
        FinishedState = 3,      ///< 结束状态  
    };

public slots:
    /**
     * 停止呼叫  
     * 实现协议接收到远程的呼叫信令后，  
     * 如果不接收对方呼叫，则调用此方法  
     * 如果通话结束，也调用此方法  
     */
    virtual int Stop()= 0;
    /**
     * 接收呼叫  
     * 实现协议接收到远程的呼叫信令后，  
     * 如果接受，则调用此方法。  
     */
    virtual int Accept()= 0;

public:
    virtual QString GetId();          ///< 得到用户ID,不含资源  
    /**
     * @brief 得到呼叫状态  
     * @return 呼叫状态  
     * @see State
     */
    virtual State GetState();
    virtual Direction GetDirection(); ///< 得到呼叫方向  
    virtual bool IsVideo();           ///< 是否是视频呼叫  
    
signals:
    /** 
     * 呼叫状态更新时触发  
     * @see CCallAction
     */
    void sigUpdate();
    /// 呼叫结束时（FinishedState）触发  
    void sigFinished(CCallObject *pCall);
    
    void sigRenderLocale(QImage frame);
    void sigRenderRemote(QImage frame);
    
protected:
    virtual int SetId(const QString szId);
    virtual int SetDirection(Direction d);
    ///播放呼叫声音  
    virtual void PlayCallSound();
    ///停止呼叫声音  
    virtual void StopCallSound();
    ///初始化视频设备与显示设备  
    virtual int OpenVideoWindow();
    virtual int CloseVideoWindow();

private slots:
    void slotFrmVideoClose();

private:
    QString m_szId;        ///< 用户 ID  
    Direction m_Direction; ///< 呼叫方向  
    QSound* m_pSound;

protected:
    bool m_bVideo;   ///< 是否包含视频  
    State m_State;   ///< 呼叫状态  
    CFrmVideo *m_pFrmVideo;
};

#endif // CALLOBJECT_H
