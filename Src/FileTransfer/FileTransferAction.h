#ifndef FILETRANSFERACTION_H
#define FILETRANSFERACTION_H

#include "Message/ChatActions/ChatAction.h"
#include "FileTransfer/FileTransfer.h"

class CFileTransferAction : public CChatAction
{
    Q_OBJECT
public:
    CFileTransferAction(QSharedPointer<CFileTransfer> file, const QString &szId, const QTime &date, const bool &me);
    virtual ~CFileTransferAction();
    virtual QString getMessage();

private slots:
    void slotUpdateHtml();
private:
    QImage drawProgressBarImg(const double &part, int w, int h, Qt::GlobalColor clrBack = Qt::white, Qt::GlobalColor clrFront = Qt::green);
    QString GetHumanReadableSize(unsigned long long size);
    QString drawProgressBar();
    QString drawImageFile();
    QString drawTop();
    QString drawBottom();
    QString drawBottomAccept();
    QString drawBottomTransfering();
    QString drawBottomCancel();
    QString drawBottomFinished();
    QString drawBottomError();


private:
    QSharedPointer<CFileTransfer> m_File;

    int m_ProgBarWidth;
    int m_ProgBarHeight;
};

#endif // FILETRANSFERACTION_H
