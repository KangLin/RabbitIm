#include "FileTransferAction.h"
#include "Tool.h"
#include <QTextEdit>
#include <QScrollBar>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QDesktopWidget>
#include <QApplication>
#include "Global/Global.h"

CFileTransferAction::CFileTransferAction(QSharedPointer<CFileTransfer> file, const QString &szId, const QTime &date, const bool &me)
  : CChatAction(me, szId, date)
{
    m_File = file;
#ifdef MOBILE
     QDesktopWidget *pDesk = QApplication::desktop();
     int nWidth;
     if(pDesk->availableGeometry().width() > pDesk->availableGeometry().height())
         nWidth = pDesk->availableGeometry().height();
     else
         nWidth = pDesk->availableGeometry().width();
     if(nWidth < 200)
         m_ProgBarWidth = nWidth - 10;
     else
         m_ProgBarWidth = nWidth - 100;
#else
    m_ProgBarWidth = 250;
#endif

    m_ProgBarHeight = 10;

    bool check = connect(file.data(), SIGNAL(sigUpdate()),this, SLOT(slotUpdateHtml()));
    Q_ASSERT(check);
}

CFileTransferAction::~CFileTransferAction()
{
    LOG_MODEL_DEBUG("CFileTransferAction", "CFileTransferAction::~CFileTransferAction");
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
        m_File.clear();
    }
}

QString CFileTransferAction::getMessage()
{
    QString content;

    //如果传输成功，且文件是图片，则直接在消息对话框中显示图片  
    if(m_File->GetError() == CFileTransfer::NoError
       && m_File->GetState() == CFileTransfer::FinishedState
       && CTool::isImageFile(m_File->GetLocalFileUrl().toLocalFile()))
    {
        return drawImageFile();
    }

    //content += "<table bgcolor='#00ffff'>";
    content += "<table>";
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
        content  += "<a href='" + m_File->GetLocalFileUrl().toLocalFile() + "'>"
                + m_File->GetLocalFileUrl().fileName()
                + "</a>";
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

    content += "<td align=right>" + tr("ETA: ") + etaTime.toString("hh:mm:ss") + "</td>";
    content += "</tr><tr><td colspan=3>";
    if(m_File->GetError() == CFileTransfer::AbortError
            || m_File->GetError() != CFileTransfer::NoError)
       content += QImage2Html(drawProgressBarImg(1, m_ProgBarWidth, m_ProgBarHeight, Qt::red, Qt::red));
    else
        content += QImage2Html(drawProgressBarImg((double)m_File->GetDoneSize()/(double)m_File->GetFileSize(), m_ProgBarWidth, m_ProgBarHeight));
    content += "</td></tr>";
    return content;
}

QImage CFileTransferAction::drawProgressBarImg(const double &part, int w, int h, Qt::GlobalColor clrBack, Qt::GlobalColor clrFront)
{
    QImage progressBar(w, h, QImage::Format_RGB16);

    QPainter qPainter(&progressBar);
    qPainter.setBrush(QBrush(clrBack));
    qPainter.setPen(Qt::white);
    qPainter.drawRect(0, 0, w, h);

    if(0 != part)
    {
        qPainter.setBrush(QBrush(clrFront));
        qPainter.setPen(Qt::green);
        qPainter.drawRect(0, 0, w * part, h);
    }
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
    QString content = "<tr>";
    content += drawAccept("rabbitim://FileTransfer?command=accept&id="
            + m_File->GetFileTranserId());

    QString szHref;
    szHref = "rabbitim://FileTransfer?command=saveas&id="
            + m_File->GetFileTranserId();
    content += drawButton(szHref, tr("Save as ..."), ":/icon/SaveAs");

    content += drawCancel("rabbitim://FileTransfer?command=cancel&id="
            + m_File->GetFileTranserId());

    content += "</tr>";
    return content;
}

QString CFileTransferAction::drawBottomTransfering()
{
    QString content = "<tr><td colspan=3 align=center>";
    content += "<a href='rabbitim://FileTransfer?command=cancel&id="
            + m_File->GetFileTranserId() + "'>";
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
    content += "<td align='center'><a href='";
#ifdef UNIX
    content += "file://";
#endif
    QFileInfo info(m_File->GetLocalFileUrl().toLocalFile());
    QString filename = info.absolutePath();
    LOG_MODEL_DEBUG("CFileTransferAction", "file:%s", qPrintable(filename));

    content += filename + "'>";
    content += tr("Open folder");
    content += "</a></td>";
    content += "<td colspan=2 align=center>";
    content += tr("Send the file has been finished.");
    content += "</td></tr>";
    return content;
}

QString CFileTransferAction::drawImageFile()
{
    QString content;
    content = "<a href='";
    content += m_File->GetLocalFileUrl().toLocalFile();
    content += "'>";
    content += "<img src='" + m_File->GetLocalFileUrl().toLocalFile() + "' alt='" 
            + m_File->GetLocalFileUrl().toString()
            + "' /></a>";
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
