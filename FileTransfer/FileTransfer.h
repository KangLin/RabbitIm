#ifndef FILETRANSFER_H_2014_10_10
#define FILETRANSFER_H_2014_10_10

#include <QObject>

class CFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CFileTransfer(QObject *parent = 0);
    virtual ~CFileTransfer();

    virtual int Accept(const QString& szFile);
    virtual int Abort();

    virtual QString GetFile();
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

signals:
    void sigFileReceived(const QString& szId, const QString& szFile, const QString& szDescription);
    /// This signal is emitted when an error is encountered while
    /// processing the transfer job.
    void sigError(CFileTransfer::Error error);

    /// This signal is emitted when the transfer job is finished.
    ///
    /// You can determine if the job completed successfully by testing whether
    /// error() returns QXmppTransferJob::NoError.
    ///
    /// Note: Do not delete the job in the slot connected to this signal,
    /// instead use deleteLater().
    void sigFinished();

    /// This signal is emitted to indicate the progress of this transfer job.
    void sigProgress(qint64 done, qint64 total);

    /// This signal is emitted when the transfer job changes state.
    void sigStateChanged(CFileTransfer::State state);
public slots:
    
};

#endif // FILETRANSFER_H_2014_10_10
