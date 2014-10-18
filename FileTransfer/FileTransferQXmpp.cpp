#include "FileTransferQXmpp.h"
#include "Global/Global.h"
#include "qxmpp/QXmppUtils.h"

CFileTransferQXmpp::CFileTransferQXmpp(QXmppTransferJob *pJob, QObject *parent) :
    CFileTransfer(parent)
{
    m_pJob = pJob;
    m_DoneSize = 0;
    m_localFile = pJob->localFileUrl();
    m_szFileName = pJob->fileName();
    m_nFileSize = pJob->fileSize();
    m_Direction = (Direction)pJob->direction();
    m_State = (State)pJob->state();
    m_Error = (Error)pJob->error();
    m_szId = QXmppUtils::jidToBareJid(pJob->jid());
    
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
    if(m_pJob)
        m_pJob->accept(szFile);
    m_localFile = QUrl::fromLocalFile(szFile);
    return nRet;
}

int CFileTransferQXmpp::Abort()
{
    if(m_pJob)
        m_pJob->abort();
    return 0;
}

QUrl CFileTransferQXmpp::GetLocalFileUrl()
{
    if(m_pJob)
        m_pJob->localFileUrl();
    return m_localFile;
}

QString CFileTransferQXmpp::GetFile()
{
    return m_szFileName;
}

qint64 CFileTransferQXmpp::GetFileSize()
{
    return m_nFileSize;
}

CFileTransfer::Direction CFileTransferQXmpp::GetDirection()
{
    return m_Direction;
}

CFileTransfer::State CFileTransferQXmpp::GetState()
{
    if(m_pJob)
        return (State)m_pJob->state();
    return m_State;
}

CFileTransfer::Error CFileTransferQXmpp::GetError()
{
    if(m_pJob)
        return (Error)m_pJob->error();
    return m_Error;
}

qint64 CFileTransferQXmpp::GetSpeed()
{
    if(m_pJob)
        return m_pJob->speed();
    return 0;
}

qint64 CFileTransferQXmpp::GetDoneSize()
{
    return m_DoneSize;
}

void CFileTransferQXmpp::slotError(QXmppTransferJob::Error error)
{
    LOG_MODEL_DEBUG("CFileTransferQXmpp", "Error:%d", error);
    m_Error = (Error)error;
    emit sigUpdate();

    if(NoError != error)
        emit sigFinished(GetId(), GetFileTranserId());
}

void CFileTransferQXmpp::slotProgress(qint64 done, qint64 total)
{
    m_DoneSize = done;
    QDateTime now = QDateTime::currentDateTime();
    if (m_LastUpdateTime.secsTo(now) < 1) //update every 1s
        return;

    m_LastUpdateTime = now;
    
    if(total != m_pJob->fileSize())
    {
        LOG_MODEL_ERROR("CFileTransferQXmpp", "file size is equet:total:%d;fileSize:%d", total, m_pJob->fileSize());
    }
    emit sigUpdate();
}

void CFileTransferQXmpp::slotStateChanged(QXmppTransferJob::State state)
{
    LOG_MODEL_DEBUG("CFileTransferQXmpp", "state:%d", state);
    m_State = (State) state;
    if(TransferState == m_State)
        m_LastUpdateTime = QDateTime::currentDateTime();
    emit sigUpdate();
    if(FinishedState == m_State)
        emit sigFinished(GetId(), GetFileTranserId());
}
