#include "ManageFileTransfer.h"
#include "Global/Global.h"
#include "Message/ChatActions/FileTransferAction.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

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

int CManageFileTransfer::ProcessCommand(const QString &szId, const QString &szCommand)
{
    int nRet = -1;
    QStringList szPara;
    szPara = szCommand.split("&");
    QString szCmd = szPara.at(0).split("=").at(1);
    QString szFileId = szPara.at(1).split("=").at(1);
    LOG_MODEL_DEBUG("CManageFileTransfer", "cmd:%s;id:%s", qPrintable(szCmd), qPrintable(szId));
    QMap<QString, QSharedPointer<CFileTransfer> >::iterator it = m_FileTransfer.find(szId);
    while (m_FileTransfer.end() != it)
    {
        if(it.value()->GetId() == szFileId)
        {
            if("accept" == szCmd)
            {
                nRet = Accept(it.value());
            }
            if("cancel" ==szCmd)
                nRet = it.value()->Abort();
            return nRet;
        }
        it++;
    }
    LOG_MODEL_DEBUG("CManageFileTransfer", "There isn't szId:%s;Fileid:%s", qPrintable(szId), qPrintable(szFileId));
    return nRet;
}

int CManageFileTransfer::Accept(QSharedPointer<CFileTransfer> file)
{
    int nRet = 0;
#ifdef MOBILE
    QDesktopWidget *pDesk = QApplication::desktop();
    QFileDialog dlg(pDesk, tr("Sava as ..."), file->GetFile(), QString());
    //dlg.setGeometry(this->rect());
    QScreen* pScreen = QApplication::primaryScreen();
    dlg.setGeometry(pScreen->availableGeometry());
    QString szFile;
    QStringList fileNames;
    if(dlg.exec())
        fileNames = dlg.selectedFiles();
    else
        return -1;
    if(fileNames.isEmpty())
        return -2;
    szFile = *fileNames.begin();
#else
    QString szFile = QFileDialog::getSaveFileName(NULL, tr("Sava as ..."),
                file->GetFile(), QString(), 0,
                QFileDialog::ReadOnly | QFileDialog::DontUseNativeDialog);
#endif
    nRet =file->Accept(szFile);
    return nRet;
}
