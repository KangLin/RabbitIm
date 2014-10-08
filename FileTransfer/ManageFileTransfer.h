#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include "FileTransfer.h"
#include <list>

class CManageFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CManageFileTransfer(QObject *parent = 0);
    virtual ~CManageFileTransfer();

public:
    int SendFile(const QString& szId, const QString &szFile, const QString& szDescription = QString());

signals:

public slots:

private:
   // std::list<QSharedPointer<CFileTransfer> > m_FileTransfer;
};

#endif // FILETRANSFER_H
