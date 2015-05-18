#ifndef FILETRANSFER_H_2014_10_10
#define FILETRANSFER_H_2014_10_10

#include <QObject>
#include <QUrl>

/**
  *@defgroup RABBITIM_INTERFACE_FILETRASFER 文件传输接口类模块  
  *@ingroup RABBITIM_INTERFACE_MANGEFILETRANSFER
  *@brief 文件传输接口类模块  
  */

/**
 * @ingroup RABBITIM_INTERFACE_FILETRASFER RABBITIM_INTERFACE
 * @brief 文件传输接口类  
 */
class CFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CFileTransfer(QObject *parent = 0);
    virtual ~CFileTransfer();

    virtual QString GetFileTranserId();//标志此对象  
    virtual QString GetId();

    /**
     * @brief 接收文件操作  
     *
     * @param szFile
     * @return int
     */
    virtual int Accept(const QString& szFile);
    /**
     * @brief 取消操作  
     *
     * @return int
     */
    virtual int Abort();

    enum Direction
    {
        IncomingDirection, ///< The file is being received.
        OutgoingDirection, ///< The file is being sent.
    };
    /// This enum is used to describe the type of error encountered by a transfer job.
    enum Error
    {
        NoError = 0,      ///< No error occurred.
        AbortError,       ///< The file transfer was aborted.
        FileAccessError,  ///< An error was encountered trying to access a local file.
        FileCorruptError, ///< The file is corrupt: the file size or hash do not match.
        ProtocolError,    ///< An error was encountered in the file transfer protocol.
    };

    /// This enum is used to describe the state of a transfer job.
    enum State
    {
        OfferState = 0,    ///< The transfer is being offered to the remote party.
        StartState = 1,    ///< The transfer is being connected.
        TransferState = 2, ///< The transfer is ongoing.
        FinishedState = 3, ///< The transfer is finished.
    };

    virtual QUrl GetLocalFileUrl();///< 得到本地文件  
    virtual QString GetFile();  ///< 得到传送的文件名  
    virtual qint64 GetFileSize();  ///< 得到文件的大小  
    virtual Direction GetDirection(); ///< 得到传输方向  
    virtual QString GetDescription(); ///< 得到文件描述
    virtual State GetState();       ///< 得到状态  
    virtual Error GetError();        ///< 得到错误  

    virtual qint64 GetSpeed();   ///< 得到传输速率  
    virtual qint64 GetDoneSize();  ///< 得到已经传输的大小  

signals:
    /**
     * @brief 当状态改变或出错时触发  
     *
     */
    void sigUpdate();
    /**
     * @brief 发送文件完成或出错或取消后触发此消息，用于管理对象删除此对象  
     *
     * @param szId：好友Id  
     * @param szFileTransferId:此对象id  
     */
    void sigFinished(const QString& szId, const QString& szFileTransferId);

public slots:

protected:
    int m_nId;//些对象ID  
    QString m_szId;//好友id  
};

#endif // FILETRANSFER_H_2014_10_10
