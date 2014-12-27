#include "ManageFileTransfer.h"
#include "Global/Global.h"
#include "FileTransferAction.h"
#include "MainWindow.h"
#include "Tool.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QMessageBox>

#undef GetMessage

CManageFileTransfer::CManageFileTransfer(QObject *parent) :
    QObject(parent)
{
}

CManageFileTransfer::~CManageFileTransfer()
{
    Clean();
    LOG_MODEL_DEBUG("CManageFileTransfer", "CManageFileTransfer::~CManageFileTransfer");
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

int CManageFileTransfer::GetFileTransfers(const QString &szId)
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
    bool check = connect(file.data(), SIGNAL(sigFinished(const QString&, const QString&)),
                         SLOT(slotFinished(const QString&, const QString&)));
    Q_ASSERT(check);
    m_FileTransfer.insertMulti(szId, file);

    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageFileTransfer", "There isn't roster:%s", szId.toStdString().c_str());
        return -1;
    }

    QSharedPointer<CFileTransferAction> action(new CFileTransferAction(file, szId, QTime::currentTime(), true));
    roster->GetMessage()->AddMessage(action);
    emit GET_CLIENT->sigMessageUpdate(szId);
    return nRet;
}

void CManageFileTransfer::slotFileReceived(const QString& szId, QSharedPointer<CFileTransfer> file)
{
    bool check = connect(file.data(), SIGNAL(sigFinished(const QString&, const QString&)),
                         SLOT(slotFinished(const QString&, const QString&)));
    Q_ASSERT(check);
    m_FileTransfer.insertMulti(szId, file);
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CManageFileTransfer", "There isn't roster:%s", szId.toStdString().c_str());
        return;
    }

    if(CTool::isImageFile(file->GetFile()))
    {
        Accept(file);
    }
    QSharedPointer<CFileTransferAction> action(new CFileTransferAction(file, szId, QTime::currentTime(), false));
    roster->GetMessage()->AddMessage(action);
    GET_MAINWINDOW->ShowTrayIconMessage(roster->GetInfo()->GetShowName() + ":", tr("Send file %1").arg(file->GetFile()));
    emit GET_CLIENT->sigMessageUpdate(szId);
}

void CManageFileTransfer::slotFinished(const QString &szId, const QString &szFileTransferId)
{
    QMap<QString, QSharedPointer<CFileTransfer> >::iterator it = m_FileTransfer.find(szId);
    while (m_FileTransfer.end() != it)
    {
        if(it.value()->GetFileTranserId() == szFileTransferId)
        {
            m_FileTransfer.erase(it);
            break;
        }
        it++;
    }
}

int CManageFileTransfer::CancelSend(const QString &szId)
{
    QMap<QString, QSharedPointer<CFileTransfer> >::iterator it = m_FileTransfer.find(szId);
    while (m_FileTransfer.end() != it)
    {
        it.value()->Abort();
        it++;
    }
    m_FileTransfer.remove(szId);
    return 0;
}

int CManageFileTransfer::ProcessCommand(const QString &szId, const QString &szCommand)
{
    int nRet = -1;
    QStringList szPara;
    szPara = szCommand.split("&");
    QString szCmd = szPara.at(0).split("=").at(1);//命令  
    QString szFileId = szPara.at(1).split("=").at(1);//文件id  
    LOG_MODEL_DEBUG("CManageFileTransfer", "cmd:%s;id:%s", qPrintable(szCmd), qPrintable(szId));
    QMap<QString, QSharedPointer<CFileTransfer> >::iterator it = m_FileTransfer.find(szId);
    while (m_FileTransfer.end() != it)
    {
        if(it.value()->GetFileTranserId() == szFileId)
        {
            if("accept" == szCmd)
                nRet = Accept(it.value());
            if("saveas" == szCmd)
                nRet = SaveAs(it.value());
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
    QString szTmp = CGlobal::Instance()->GetDirReceiveFile()//QStandardPaths::writableLocation(QStandardPaths::TempLocation) 
            + QDir::separator() 
            + file->GetFile();
    nRet = file->Accept(szTmp);
    return nRet;
}

int CManageFileTransfer::SaveAs(QSharedPointer<CFileTransfer> file)
{
    int nRet = 0;
    QString szFile;
    QString szDir = CGlobal::Instance()->GetDirReceiveFile()
            + QDir::separator() + file->GetFile();

    szFile = CTool::FileDialog(NULL, szDir, QString(), tr("Save as"));
    if(szFile.isEmpty())
        return -1;
    QFile f(szFile);
    if(f.exists())
    {
        if(QMessageBox::No ==
                QMessageBox::warning(NULL, tr("Save as"), 
                                     tr("File is exists. Do you save it?"),
                                     QMessageBox::Ok, 
                                     QMessageBox::No))
        {
            return 0;
        }
    }
    nRet = file->Accept(szFile);
    return nRet;
}
