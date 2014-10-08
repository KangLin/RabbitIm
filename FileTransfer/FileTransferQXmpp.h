#ifndef FILETRANSFERQXMPP_H
#define FILETRANSFERQXMPP_H

#include <QObject>
#include "FileTransfer.h"

class CFileTransferQXmpp : public CFileTransfer
{
    Q_OBJECT
public:
    explicit CFileTransferQXmpp(QObject *parent = 0);

    int SendFile(const QString& szId, const QString &szFile, const QString& szDescription = QString());
    int Accept(const QString& szFile);
    int Abort();
signals:
    
public slots:
    
};

#endif // FILETRANSFERQXMPP_H
