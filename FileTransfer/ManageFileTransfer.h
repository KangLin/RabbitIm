#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>

#ifdef QXMPP
#include "FileTransferQXmpp.h"
#else
#include "FileTransfer.h"
#endif

class CManageFileTransfer : public QObject
{
    Q_OBJECT

public:
    explicit CManageFileTransfer(QObject *parent = 0);
    virtual ~CManageFileTransfer();

public:
    /**
     * @brief 用户登录成功后调用,用于初始化工作  
     *
     * @param szId:登录用户名  
     * @return int
     */
    virtual int Init(QString szId);
    /**
     * @brief 用户登出时调用,用于清理工作   
     *
     * @return int
     */
    virtual int Clean();

    int SendFile(const QString& szId, const QString &szFile, const QString& szDescription = QString());

    /**
     * @brief 得到好友正在发送文件的个数  
     *
     * @param szId：好友id  
     * @return int：正在发送文件的个数  
     */
    int GetFileTransfer(const QString &szId);

    int CancelSend(const QString& szId);
    int CancelSend(QSharedPointer<CFileTransfer> file);

    int ProcessCommand(const QString& szId, const QString& szCommand);

private:
    int Accept(QSharedPointer<CFileTransfer> file);

signals:

public slots:
    /**
     * @brief 文件接收时触发  
     *
     * @param file
     */
    void slotFileReceived(const QString& szId, QSharedPointer<CFileTransfer> file);

private:
    QMap<QString, QSharedPointer<CFileTransfer> > m_FileTransfer;
};

#endif // FILETRANSFER_H
