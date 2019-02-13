#ifndef FILETRANSFERQXMPP_H
#define FILETRANSFERQXMPP_H

#include <QObject>
#include "FileTransfer/FileTransfer.h"
#include "qxmpp/QXmppTransferManager.h"
#include <QDateTime>

class CFileTransferQXmpp : public CFileTransfer
{
    Q_OBJECT
public:
    explicit CFileTransferQXmpp(QXmppTransferJob* pJob, QObject *parent = 0);
    virtual ~CFileTransferQXmpp();

    virtual int Accept(const QString& szFile);
    virtual int Abort();

    virtual QUrl GetLocalFileUrl();
    virtual QString GetFile();
    virtual qint64 GetFileSize();
    virtual Direction GetDirection();
    virtual QString GetDescription();
    virtual State GetState();
    virtual Error GetError();
    virtual qint64 GetSpeed();
    virtual qint64 GetDoneSize();
signals:
    
public slots:
    void slotError(QXmppTransferJob::Error error);
    void slotStateChanged(QXmppTransferJob::State state);
    void slotProgress(qint64 done, qint64 total);
    void slotFinished();
    
private:
     QXmppTransferJob *m_pJob;
     qint64 m_DoneSize;
     QUrl m_localFile;
     QString m_szFileName;
     qint64 m_nFileSize;
     Direction m_Direction;
     CFileTransfer::State m_State;
     Error m_Error;

     QDateTime m_LastUpdateTime;
};

#endif // FILETRANSFERQXMPP_H
