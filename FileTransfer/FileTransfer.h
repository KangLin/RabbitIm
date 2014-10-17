#ifndef FILETRANSFER_H_2014_10_10
#define FILETRANSFER_H_2014_10_10

#include <QObject>
#include <QUrl>

class CFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CFileTransfer(QObject *parent = 0);
    virtual ~CFileTransfer();

    virtual QString GetId();//标志此对象  
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

public slots:

private:
    int m_nId;//些对象ID  
};

#endif // FILETRANSFER_H_2014_10_10
