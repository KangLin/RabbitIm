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
    explicit CCallObject(const QString &szId = QString(), bool bVideo = false, QObject *parent = 0);
    virtual ~CCallObject();

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
     * @brief 主动呼叫,如果派生类要重载它，必需先调用 CallObject::Call()  
     *
     * @param szId：用户ID  
     * @param bVideo:是否是视频呼叫  
     * @return int
     */
    virtual int Call();
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
    virtual bool IsVideo();           ///< 是否是视频呼叫  
    /**
     * @brief 设置错误信息  
     * @param nError：错误号  
     * @param szError：错误信息  
     * @return 
     */
    int SetError(int nError, const QString &szError);
    /**
     * @brief 得到错误信息  
     * @param szError：错误信息  
     * @return nError：错误号  
     */
    int GetError(QString &szError);
    
signals:
    /** 
     * 呼叫状态更新时触发。
     * 派生类实现时，有状态改变，不直接触发，
     * 而应该调用  slotChanageState 
     * @see CCallAction
     */
    void sigUpdate();
    /// 呼叫结束时（FinishedState）触发  
    void sigFinished(CCallObject *pCall);
    
private:
signals:
    // 显示视频帧，仅用于CallObject继承类, 
    // QImage 格仅为 Format_ARGB32 或 Format_RGB32  
    void sigRenderLocale(QImage frame);
    void sigRenderRemote(QImage frame);
    
protected:
    virtual int SetId(const QString szId);
    enum Direction
    {
        IncomingDirection, ///< The call is incoming.
        OutgoingDirection, ///< The call is outgoing.
    };
    virtual int SetDirection(Direction d);
    virtual Direction GetDirection(); ///< 得到呼叫方向  
    ///播放呼叫声音  
    virtual void PlayCallSound();
    ///停止呼叫声音  
    virtual void StopCallSound();
    ///打开视频显示窗口  
    virtual int OpenVideoWindow();
    ///关闭视频显示窗口  
    virtual int CloseVideoWindow();
    bool IsMonitor();//是否是监控模式  

protected slots:
    void slotChanageState(CCallObject::State state);

private slots:
    void slotFrmVideoClose();

private:
    QString m_szError;
    int m_nError;
    QString m_szId;        ///< 用户 ID  
    Direction m_Direction; ///< 呼叫方向  
    QSound* m_pSound;      ///< 铃音  

protected:
    bool m_bVideo;          ///< 是否包含视频  
    State m_State;          ///< 呼叫状态  
    CFrmVideo *m_pFrmVideo; ///< 视频显示窗口  
};

#endif // CALLOBJECT_H
