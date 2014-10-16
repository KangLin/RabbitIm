#include "FileTransferAction.h"
#include <QTextEdit>
#include <QScrollBar>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QDir>

CFileTransferAction::CFileTransferAction(QSharedPointer<CFileTransfer> file, const QString &author, const QDate &date, const bool &me)
  : CChatAction(me, author, date)
{
    m_File = file;
    m_pEdit = NULL;
    m_ProgBarHeight = 10;
    m_ProgBarWidth = 250;

    bool check = connect(file.data(), SIGNAL(sigUpdate()),this, SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
}

CFileTransferAction::~CFileTransferAction()
{
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

void CFileTransferAction::slotUpdateHtml()
{
    if (m_Cursor.isNull() || !m_pEdit)
        return;

    // save old slider value
    int vSliderVal = m_pEdit->verticalScrollBar()->value();

    // update content
    int pos = m_Cursor.selectionStart();
    m_Cursor.removeSelectedText();
    m_Cursor.setKeepPositionOnInsert(false);
    m_Cursor.insertHtml(getContent());
    m_Cursor.setKeepPositionOnInsert(true);
    int end = m_Cursor.position();
    m_Cursor.setPosition(pos);
    m_Cursor.setPosition(end, QTextCursor::KeepAnchor);

    // restore old slider value
    m_pEdit->verticalScrollBar()->setValue(vSliderVal);

    // Free our ressources if we'll never need to update again
    if(m_File->GetError() == CFileTransfer::AbortError
            || m_File->GetState() == CFileTransfer::FinishedState)
    {
        m_Cursor = QTextCursor();
    }
}

QString CFileTransferAction::getMessage()
{
    QString content;

    content += "<table bgcolor='#00ffff'>";
    content += drawTop();
    content += drawProgressBar();
    content += drawBottom();
    content += "</table>";
    return content;
}

QString CFileTransferAction::drawTop()
{
    QString content;
    content = "<tr><td colspan='3' align='center'>";
    if(m_File->GetState() == CFileTransfer::FinishedState && m_File->GetError() == CFileTransfer::NoError)
        content  += "<a href='" + m_File->GetLocalFileUrl().toString() + "'>" + m_File->GetLocalFileUrl().fileName() + "</a>";
    else
        content  += "<p>" + m_File->GetFile() + "</p>";
    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawProgressBar()
{
    QString content;
    content += "<tr><td>" + GetHumanReadableSize(m_File->GetFileSize()) + "</td>";
    content += "<td align=center>" + GetHumanReadableSize(m_File->GetSpeed()) + "/s</td>";

    int etaSecs = 0;
    if(m_File->GetSpeed())
        etaSecs = (m_File->GetFileSize() - m_File->GetDoneSize()) / m_File->GetSpeed();
    QTime etaTime(0,0);
    etaTime = etaTime.addSecs(etaSecs);

    content += "<td align=right>" + tr("ETA: ") + etaTime.toString("mm:ss") + "</td>";
    content += "</tr><tr><td colspan=3>";
    content += QImage2Html(drawProgressBarImg((double)m_File->GetDoneSize()/(double)m_File->GetFileSize(), m_ProgBarWidth, m_ProgBarHeight));
    content += "</td></tr>";
    return content;
}

QImage CFileTransferAction::drawProgressBarImg(const double &part, int w, int h)
{
    QImage progressBar(w, h, QImage::Format_Mono);

    QPainter qPainter(&progressBar);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::black);
    qPainter.drawRect(0, 0, w - 1, h - 1);

    //qPainter.setBrush(Qt::SolidPattern);
    qPainter.setBrush(QBrush(Qt::green));
    qPainter.setPen(Qt::black);
    qPainter.drawRect(1, 0, (w - 2) * (part), h - 1);

    return progressBar;
}

QString CFileTransferAction::drawBottom()
{
    QString szContent;
    if(m_File->GetError() == CFileTransfer::AbortError)
    {
        szContent = drawBottomCancel();
        return szContent;
    }
    else if(m_File->GetError() != CFileTransfer::NoError)
    {
        szContent = drawBottomError();
        return szContent;
    }
    if(m_File->GetState() == CFileTransfer::OfferState && m_File->GetDirection() == CFileTransfer::IncomingDirection)
    {
        szContent = drawBottomAccept();
        return szContent;
    }
    if(m_File->GetState() == CFileTransfer::TransferState 
            || (m_File->GetState() == CFileTransfer::OfferState 
                && m_File->GetDirection() == CFileTransfer::OutgoingDirection)
            || m_File->GetState() == CFileTransfer::StartState)
    {
        szContent = drawBottomTransfering();
        return szContent;
    }
    if(m_File->GetState() == CFileTransfer::FinishedState)
    {
        szContent = drawBottomFinished();
        return szContent;
    }
    return szContent;
}

QString CFileTransferAction::drawBottomAccept()
{
    QString content = "<tr><td  align=center>";
    content += "<a href='rabbitim://FileTransfer?command=accept&id="
            + m_File->GetId() + "'>";
    content += QImage2Html(QImage(":/icon/Accept", "png"), 16, 16);
    content += tr("Accept") + "</a>";

    content += "</td><td />";
    content += "<td align=center>";
    content += "<a href='rabbitim://FileTransfer?command=cancel&id="
            + m_File->GetId() + "'>";
    content += QImage2Html(QImage(":/icon/Cancel", "png"), 16, 16)
            + tr("Cancel") + "</a>";

    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawBottomTransfering()
{
    QString content = "<tr><td colspan=3 align=center>";
    content += "<a href='rabbitim://FileTransfer?command=cancel&id="
            + m_File->GetId() + "'>";
    content += QImage2Html(QImage(":/icon/Cancel", "png"), 16, 16)
            + tr("Cancel") + "</a>";

    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawBottomCancel()
{
    QString content = "<tr><td colspan=3 align=center>";
    content += tr("Send the file has been canceled.");
    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawBottomFinished()
{
    QString content = "<tr>";
    content += "<td align='center'><a href='file://";
    
    QFileInfo info(m_File->GetLocalFileUrl().toLocalFile());
    QString filename = info.absolutePath();

    content += filename + "'>";
    content += tr("Open folder");
    content += "</a></td>";
    content += "<td colspan=2 align=center>";
    content += tr("Send the file has been finished.");
    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawBottomError()
{
    QString content = "<tr><td colspan=3 align=center>";
    content += tr("File sending error.");
    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::GetHumanReadableSize(unsigned long long size)
{
    static const char* suffix[] = {"B","kiB","MiB","GiB","TiB"};
    int exp = 0;
    if (size)
        exp = std::min( (int) (log(size) / log(1024)), (int) (sizeof(suffix) / sizeof(suffix[0]) - 1));
    return QString().setNum(size / pow(1024, exp),'f',2).append(suffix[exp]);
}
