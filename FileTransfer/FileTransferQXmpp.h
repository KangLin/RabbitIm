#ifndef FILETRANSFERQXMPP_H
#define FILETRANSFERQXMPP_H

#include <QObject>
#include "FileTransfer.h"
#include "qxmpp/QXmppTransferManager.h"

class CFileTransferQXmpp : public CFileTransfer
{
    Q_OBJECT
public:
    explicit CFileTransferQXmpp(QXmppTransferJob* pJob, QObject *parent = 0);

    virtual int Accept(const QString& szFile);
    virtual int Abort();

    virtual QString GetFile();

signals:
    
public slots:
    
private:
     QXmppTransferJob *m_pJob;
};

#endif // FILETRANSFERQXMPP_H
