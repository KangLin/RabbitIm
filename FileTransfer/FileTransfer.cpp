#include "FileTransfer.h"
#include "Global/Global.h"

CFileTransfer::CFileTransfer(QObject *parent) :
    QObject(parent)
{
}

CFileTransfer::~CFileTransfer()
{
}

int CFileTransfer::SendFile(const QString &szId, const QString &szFile, const QString &szDescription)
{
    Q_UNUSED(szId);
    Q_UNUSED(szFile);
    Q_UNUSED(szDescription);
    LOG_MODEL_WARNING("CFileTransfer", "The SendFile function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
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
