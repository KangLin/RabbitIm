#include "FileTransfer.h"
#include "Global/Global.h"

CFileTransfer::CFileTransfer(QObject *parent) :
    QObject(parent)
{
}

CFileTransfer::~CFileTransfer()
{
}

int CFileTransfer::Accept(const QString &szFile)
{
    Q_UNUSED(szFile);
    LOG_MODEL_WARNING("CFileTransfer", "The Accept function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CFileTransfer::Abort()
{
    LOG_MODEL_WARNING("CFileTransfer", "The Abort function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

QUrl CFileTransfer::GetLocalFileUrl()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetLocalFileUrl function must be implemented by derived classes");
    Q_ASSERT(false);
    return QUrl();
}

QString CFileTransfer::GetFile()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetFile function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

qint64 CFileTransfer::GetFileSize()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetFileSize function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

CFileTransfer::Direction CFileTransfer::GetDirection()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetDirection function must be implemented by derived classes");
    Q_ASSERT(false);
    return IncomingDirection;
}

CFileTransfer::State CFileTransfer::GetState()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetState function must be implemented by derived classes");
    Q_ASSERT(false);
    return OfferState;
}

CFileTransfer::Error CFileTransfer::GetError()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetError function must be implemented by derived classes");
    Q_ASSERT(false);
    return NoError;
}
