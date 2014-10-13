#include "FileTransferAction.h"

#include <QTextEdit>
#include <QScrollBar>
#include <QImage>
#include <QPainter>

CFileTransferAction::CFileTransferAction(QSharedPointer<CFileTransfer> file, const QString &author, const QDate &date, const bool &me)
  : CChatAction(me, author, date)
{
    m_File = file;
    m_pEdit = NULL;
    m_dbLastBytesSent = 0;
    m_dbTotalBytes = 0;
    m_dbSpeed = 0;
    m_dbFileSize = 0;
    m_dbEta = 0;
    m_ProgBarHeight = 9;
    m_ProgBarWidth = 250;
    m_szFile = file->GetFile();

    //connect(w, &FileTransferInstance::stateUpdated, this, &FileTransferAction::updateHtml);
}

CFileTransferAction::~CFileTransferAction()
{
}

QString CFileTransferAction::getMessage()
{
    QString content;
    QString progrBar = "<img src=\"data:progressbar./png;base64," 
            + QImage2base64(drawProgressBarImg(m_dbLastBytesSent/m_dbTotalBytes, m_ProgBarWidth, m_ProgBarHeight))
            + "\">";

    content  = "<p>" + m_szFile + "</p>";
    content += "<table cellspacing='0'><tr>";
    content += "<td>" + GetHumanReadableSize(m_dbFileSize) + "</td>";
    content += "<td align=center>" + GetHumanReadableSize(m_dbSpeed) + "</td>";
    content += "<td align=right>ETA: " + QString::number(m_dbEta) + "</td>";
    content += "</tr><tr><td colspan=3>";
    content += progrBar;
    content += "</td></tr></table>";
    return content;
}

void CFileTransferAction::setup(QTextCursor cursor, QTextEdit *textEdit)
{
    m_Cursor = cursor;
    m_Cursor.setKeepPositionOnInsert(true);
    int end=m_Cursor.selectionEnd();
    m_Cursor.setPosition(m_Cursor.position());
    m_Cursor.setPosition(end, QTextCursor::KeepAnchor);

    m_pEdit = textEdit;
}

void CFileTransferAction::updateHtml()
{
    if (m_Cursor.isNull() || !m_pEdit)
        return;

    // save old slider value
    int vSliderVal = m_pEdit->verticalScrollBar()->value();

    // update content
    int pos = m_Cursor.selectionStart();
    m_Cursor.removeSelectedText();
    m_Cursor.setKeepPositionOnInsert(false);
    m_Cursor.insertHtml(getMessage());
    m_Cursor.setKeepPositionOnInsert(true);
    int end = m_Cursor.position();
    m_Cursor.setPosition(pos);
    m_Cursor.setPosition(end, QTextCursor::KeepAnchor);

    // restore old slider value
    m_pEdit->verticalScrollBar()->setValue(vSliderVal);

    // Free our ressources if we'll never need to update again
    /*if (w->getState() == FileTransferInstance::TransfState::tsCanceled
            || w->getState() == FileTransferInstance::TransfState::tsFinished)
    {
        name.clear();
        name.squeeze();
        date.clear();
        date.squeeze();
        cur = QTextCursor();
    }*/
}

QImage CFileTransferAction::drawProgressBarImg(const double &part, int w, int h)
{
    QImage progressBar(w, h, QImage::Format_Mono);

    QPainter qPainter(&progressBar);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::black);
    qPainter.drawRect(0, 0, w - 1, h - 1);

    qPainter.setBrush(Qt::SolidPattern);
    qPainter.setPen(Qt::black);
    qPainter.drawRect(1, 0, (w - 2) * (part), h - 1);

    return progressBar;
}

QString CFileTransferAction::GetHumanReadableSize(unsigned long long size)
{
    static const char* suffix[] = {"B","kiB","MiB","GiB","TiB"};
    int exp = 0;
    if (size)
        exp = std::min( (int) (log(size) / log(1024)), (int) (sizeof(suffix) / sizeof(suffix[0]) - 1));
    return QString().setNum(size / pow(1024, exp),'f',2).append(suffix[exp]);
}
