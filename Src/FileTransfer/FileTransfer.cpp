#include "FileTransfer.h"
#include "Global/Global.h"

int g_CFileTransferId = 0;

CFileTransfer::CFileTransfer(QObject *parent) :
    QObject(parent)
{
    m_nId = g_CFileTransferId++;
}

CFileTransfer::~CFileTransfer()
{
    qDebug("CFileTransfer::~CFileTransfer:id:%d", m_nId);
}

QString CFileTransfer::GetFileTranserId()
{
    return QString::number(m_nId);
}

QString CFileTransfer::GetId()
{
    return m_szId;
}

int CFileTransfer::Accept(const QString &szFile)
{
    Q_UNUSED(szFile);
    qWarning("The Accept function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

int CFileTransfer::Abort()
{
    qWarning("The Abort function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

QUrl CFileTransfer::GetLocalFileUrl()
{
    qWarning("The GetLocalFileUrl function must be implemented by derived classes");
    Q_ASSERT(false);
    return QUrl();
}

QString CFileTransfer::GetFile()
{
    qWarning("The GetFile function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

qint64 CFileTransfer::GetFileSize()
{
    qWarning("The GetFileSize function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

CFileTransfer::Direction CFileTransfer::GetDirection()
{
    qWarning("The GetDirection function must be implemented by derived classes");
    Q_ASSERT(false);
    return IncomingDirection;
}

QString CFileTransfer::GetDescription()
{
    qWarning("The GetDescription function must be implemented by derived classes");
    Q_ASSERT(false);
    return QString();
}

CFileTransfer::State CFileTransfer::GetState()
{
    qWarning("The GetState function must be implemented by derived classes");
    Q_ASSERT(false);
    return OfferState;
}

CFileTransfer::Error CFileTransfer::GetError()
{
    qWarning("The GetError function must be implemented by derived classes");
    Q_ASSERT(false);
    return NoError;
}

qint64 CFileTransfer::GetSpeed()
{
    qWarning("The GetSpeed function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

qint64 CFileTransfer::GetDoneSize()
{
    qWarning("The GetDoneSize function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}
