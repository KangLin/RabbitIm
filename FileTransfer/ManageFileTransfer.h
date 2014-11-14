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

    /**
     * @brief 向用户发送文件  
     *
     * @param szId：用户id  
     * @param szFile：要发送的文件名  
     * @param szDescription：描述  
     * @return int：成功返回0，失败返回非0  
     */
    int SendFile(const QString& szId, const QString &szFile, const QString& szDescription = QString());

    /**
     * @brief 得到好友正在发送文件的个数  
     *
     * @param szId：好友id  
     * @return int：正在发送文件的个数  
     */
    int GetFileTransfers(const QString &szId);

    /**
     * @brief 取消息用户发送的文件  
     *
     * @param szId：用户id  
     * @return int
     */
    int CancelSend(const QString& szId);

    /**
     * @brief 根据命令串执行操作  
     *
     * @param szId
     * @param szCommand
     * @return int
     */
    int ProcessCommand(const QString& szId, const QString& szCommand);

private:
    int Accept(QSharedPointer<CFileTransfer> file);
    int SaveAs(QSharedPointer<CFileTransfer> file);

signals:

public slots:
    /**
     * @brief 文件接收时触发  
     *
     * @param file
     */
    void slotFileReceived(const QString& szId, QSharedPointer<CFileTransfer> file);
    void slotFinished(const QString& szId, const QString& szFileTransferId);

private:
    QMap<QString, QSharedPointer<CFileTransfer> > m_FileTransfer;
};

#endif // FILETRANSFER_H
