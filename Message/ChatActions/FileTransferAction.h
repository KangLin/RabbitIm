#ifndef FILETRANSFERACTION_H
#define FILETRANSFERACTION_H

#include "ChatAction.h"
#include "FileTransfer/FileTransfer.h"

class CFileTransferAction : public CChatAction
{
    Q_OBJECT
public:
    CFileTransferAction(QSharedPointer<CFileTransfer> file, const QString &author, const QDate &date, const bool &me);
    virtual ~CFileTransferAction();
    virtual QString getMessage();
    virtual void setup(QTextCursor cursor, QTextEdit* textEdit) override;

private slots:
    void slotUpdateHtml();
private:
    QImage drawProgressBarImg(const double &part, int w, int h);
    QString GetHumanReadableSize(unsigned long long size);
    QString drawProgressBar();
    QString drawTop();
    QString drawBottom();
    QString drawBottomAccept();
    QString drawBottomTransfering();
    QString drawBottomCancel();
    QString drawBottomFinished();
    QString drawBottomError();

private:
    QSharedPointer<CFileTransfer> m_File;
    QTextCursor m_Cursor;
    QTextEdit* m_pEdit;

    int m_ProgBarWidth;
    int m_ProgBarHeight;
};

#endif // FILETRANSFERACTION_H
