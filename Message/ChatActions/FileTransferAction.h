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
    void updateHtml();
private:
    QImage drawProgressBarImg(const double &part, int w, int h);
    QString GetHumanReadableSize(unsigned long long size);

private:
    QSharedPointer<CFileTransfer> m_File;
    QTextCursor m_Cursor;
    QTextEdit* m_pEdit;

    QString m_szFile;
    int m_ProgBarWidth;
    int m_ProgBarHeight;
    double m_dbFileSize;
    double m_dbLastBytesSent;
    double m_dbTotalBytes;
    double m_dbSpeed;
    double m_dbEta;//estimated time of arrival
};

#endif // FILETRANSFERACTION_H
