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

QString CFileTransfer::GetFile()
{
    LOG_MODEL_WARNING("CFileTransfer", "The GetFile function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}
