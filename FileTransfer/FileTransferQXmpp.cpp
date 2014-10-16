#include "FileTransferQXmpp.h"
#include "Global/Global.h"

CFileTransferQXmpp::CFileTransferQXmpp(QXmppTransferJob *pJob, QObject *parent) :
    CFileTransfer(parent)
{
    m_pJob = pJob;
    m_DoneSize = 0;
    bool check = false;
    check = connect(pJob, SIGNAL(error(QXmppTransferJob::Error)), 
                    SLOT(slotError(QXmppTransferJob::Error)));
    Q_ASSERT(check);
    check = connect(pJob, SIGNAL(stateChanged(QXmppTransferJob::State)),
                    SLOT(slotStateChanged(QXmppTransferJob::State)));
    Q_ASSERT(check);
    check = connect(pJob, SIGNAL(progress(qint64,qint64)),
                    SLOT(slotProgress(qint64,qint64)));
    Q_ASSERT(check);
}

int CFileTransferQXmpp::Accept(const QString &szFile)
{
    int nRet = 0;
    m_pJob->setLocalFileUrl(szFile);
    m_pJob->accept(szFile);
    return nRet;
}

int CFileTransferQXmpp::Abort()
{
    m_pJob->abort();
    return 0;
}

QUrl CFileTransferQXmpp::GetLocalFileUrl()
{
    return m_pJob->localFileUrl();
}

QString CFileTransferQXmpp::GetFile()
{
    return m_pJob->fileName();
}

qint64 CFileTransferQXmpp::GetFileSize()
{
    return m_pJob->fileSize();
}

CFileTransfer::Direction CFileTransferQXmpp::GetDirection()
{
    return (Direction) m_pJob->direction();
}

CFileTransfer::State CFileTransferQXmpp::GetState()
{
    return (State)m_pJob->state();
}

CFileTransfer::Error CFileTransferQXmpp::GetError()
{
    return (Error) m_pJob->error();
}

qint64 CFileTransferQXmpp::GetSpeed()
{
    return m_pJob->speed();
}

qint64 CFileTransferQXmpp::GetDoneSize()
{
    return m_DoneSize;
}

void CFileTransferQXmpp::slotError(QXmppTransferJob::Error error)
{
    LOG_MODEL_DEBUG("CFileTransferQXmpp", "Error:%d", error);
    emit sigUpdate();
}

void CFileTransferQXmpp::slotProgress(qint64 done, qint64 total)
{
    m_DoneSize = done;
    if(total != m_pJob->fileSize())
    {
        LOG_MODEL_ERROR("CFileTransferQXmpp", "file size is equet:total:%d;fileSize:%d", total, m_pJob->fileSize());
    }
    emit sigUpdate();
}

void CFileTransferQXmpp::slotStateChanged(QXmppTransferJob::State state)
{
    LOG_MODEL_DEBUG("CFileTransferQXmpp", "state:%d", state);
    emit sigUpdate();
}
