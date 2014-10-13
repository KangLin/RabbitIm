#include "ManageFileTransfer.h"
#include "Global/Global.h"
#include "Message/ChatActions/FileTransferAction.h"

CManageFileTransfer::CManageFileTransfer(QObject *parent) :
    QObject(parent)
{
}

CManageFileTransfer::~CManageFileTransfer()
{
    Clean();
}

int CManageFileTransfer::Init(QString szId)
{
    bool check = false;
    check = connect(GET_CLIENT.data(), SIGNAL(sigFileReceived(QString,QSharedPointer<CFileTransfer>)),
                    SLOT(slotFileReceived(QString,QSharedPointer<CFileTransfer>)));
    Q_ASSERT(check);
            
    return 0;
}

int CManageFileTransfer::Clean()
{
    GET_CLIENT->disconnect(this);
    m_FileTransfer.clear();
    return 0;
}

int CManageFileTransfer::GetFileTransfer(const QString &szId)
{
    QList<QSharedPointer<CFileTransfer> > files = m_FileTransfer.values(szId);
    return files.size();
}

int CManageFileTransfer::SendFile(const QString &szId, const QString &szFile, const QString &szDescription)
{
    int nRet = 0;
    QSharedPointer<CFileTransfer> file;
    file = GET_CLIENT->SendFile(szId, szFile, szDescription);
    if(file.isNull())
    {
        return -1;
    }
    m_FileTransfer.insertMulti(szId, file);

    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageFileTransfer", "There isn't roster:%s", szId.toStdString().c_str());
        return -1;
    }

    QSharedPointer<CFileTransferAction> action(new CFileTransferAction(file, szId, QDate::currentDate(), true));
    roster->GetMessage()->AddMessage(action);
    emit GET_CLIENT->sigMessageUpdate(szId);
    return nRet;
}

void CManageFileTransfer::slotFileReceived(const QString& szId, QSharedPointer<CFileTransfer> file)
{
    m_FileTransfer.insertMulti(szId, file);
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageFileTransfer", "There isn't roster:%s", szId.toStdString().c_str());
        return;
    }
    QSharedPointer<CFileTransferAction> action(new CFileTransferAction(file, szId, QDate::currentDate(), false));
    roster->GetMessage()->AddMessage(action);
    emit GET_CLIENT->sigMessageUpdate(szId);
}

int CManageFileTransfer::CancelSend(QSharedPointer<CFileTransfer> file)
{
    return 0;
}

int CManageFileTransfer::CancelSend(const QString &szId)
{
    m_FileTransfer.remove(szId);
    return 0;
}
