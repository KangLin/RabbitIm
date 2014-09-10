#include <QFileDialog>
#include "FrmSendFile.h"
#include "ui_FrmSendFile.h"
#include "../../Global/Global.h"
#include "../../Tool.h"

CFrmSendFile::CFrmSendFile(QXmppTransferJob &job, bool isSend, QWidget *parent)
    :QFrame(parent)
    ,m_job(job)
    ,m_type(E_RECIEVE)
{
    ui = new Ui::CFrmSendFile;
    ui->setupUi(this);
    QString text = m_job.fileName();
    QFontMetrics fontM(ui->label_file->font());
    text = fontM.elidedText(text,Qt::ElideMiddle,200);
    ui->label_file->setText(text);
    ui->label_speed->setText("");

    ui->progressBar->setRange(0,100);
    initConnect();
    if(isSend)
    {
        ui->btn_accept->setHidden(true);
        ui->btn_abort->setText("cancel");
        ui->btn_saveAs->setHidden(true);
    }
    LOG_MODEL_INFO("CFrmSendFile","job.fileInfo().description() %s",job.fileInfo().description().toLocal8Bit().data());
    if(job.fileInfo().description() == "image")
    {
        m_isImg = true;
        m_saveFilePath = QDateTime::currentDateTime().toString("yyyymmddhhssdd%1").arg(job.fileName());
        m_job.accept(m_saveFilePath);
    }
    else
    {
        m_saveFilePath = job.fileName();
        m_isImg = false;
    }
}

CFrmSendFile::~CFrmSendFile()
{
    delete ui;
    ui = NULL;
}

void CFrmSendFile::setId(const QString &id)
{
    m_id = id;
}

void CFrmSendFile::closeEvent(QCloseEvent *e)
{
    emit sigClosed();
    return QFrame::closeEvent(e);
}

void CFrmSendFile::initConnect()
{
//    connect(&m_job,SIGNAL(finished()),this,SLOT(close()));
    connect(&m_job,SIGNAL(progress(qint64,qint64)),
            this,SLOT(onFileReceiveProgress(qint64,qint64)));
    connect(&m_job,SIGNAL(error(QXmppTransferJob::Error)),
            this,SLOT(onError(QXmppTransferJob::Error)));
    connect(&m_job,SIGNAL(stateChanged(QXmppTransferJob::State)),
            this,SLOT(onStateChanged(QXmppTransferJob::State)));
    //================================
    connect(&m_job,SIGNAL(localFileUrlChanged(QUrl)),
            this,SLOT(onLocalFileUrlChanged(QUrl)));
    connect(&m_job,SIGNAL(logMessage(QXmppLogger::MessageType,QString)),
            this,SLOT(onLogMessage(QXmppLogger::MessageType,QString)));
    connect(&m_job,SIGNAL(setGauge(QString,double)),
            this,SLOT(onSetGauge(QString,double)));
    connect(&m_job,SIGNAL(updateCounter(QString,qint64)),
            this,SLOT(onUpdateCounter(QString,qint64)));
}

void CFrmSendFile::onFileReceiveProgress(qint64 done, qint64 total)
{
    if(total != 0)
    {
        ui->progressBar->setValue(done * 100 / total);
        ui->progressBar->setHidden(false);
        QString spd = CTool::GetSizeString(m_job.speed());
        QString sDone = CTool::GetSizeString(done);
        QString sTotal = CTool::GetSizeString(total);
        QString text = QString("speed %1 (%2/%3)").arg(spd).arg(sDone).arg(sTotal);
        QFontMetrics fontM(ui->label_speed->font());
        text = fontM.elidedText(text,Qt::ElideMiddle,200);
//        qDebug()<<"text "<<text;
        ui->label_speed->setText(text);
    }
}

void CFrmSendFile::onFinished()
{
    if(m_isImg)
    {
//        EpPublic* p = EpPublic::instance();
//        emit sigImgRecieved(p->getReceiveDefaultSaveDir().absolutePath() + QDir::separator() +m_job.fileName());
    }
    this->close();
}

void CFrmSendFile::onError(QXmppTransferJob::Error error)
{
    LOG_MODEL_ERROR("CFrmSendFile","error = %d",error);
    switch(error)
    {
    case QXmppTransferJob::AbortError:
    {
        LOG_MODEL_ERROR("CFrmSendFile","The file transfer was aborted.");
        this->close();
        break;
    }
    case QXmppTransferJob::FileAccessError:
    {
        LOG_MODEL_ERROR("CFrmSendFile","An error was encountered trying to access a local file.");
        this->close();
        break;
    }
    case QXmppTransferJob::FileCorruptError:
    {
        LOG_MODEL_ERROR("CFrmSendFile","The file is corrupt: the file size or hash do not match.");
        this->close();
        break;
    }
    case QXmppTransferJob::ProtocolError:
    {
        LOG_MODEL_ERROR("CFrmSendFile","An error was encountered in the file transfer protocol.");
        this->close();
        break;
    }
    default:
        break;
    }
}

void CFrmSendFile::onStateChanged(QXmppTransferJob::State state)
{
    LOG_MODEL_INFO("CFrmSendFile","state = %d",state);
    switch(state)
    {
    case QXmppTransferJob::OfferState:
    {
        LOG_MODEL_INFO("CFrmSendFile","The transfer is being offered to the remote party.");
        break;
    }
    case QXmppTransferJob::StartState:
    {
        LOG_MODEL_INFO("CFrmSendFile","The transfer is being connected.");
        break;
    }
    case QXmppTransferJob::TransferState:
    {
        LOG_MODEL_INFO("CFrmSendFile","The transfer is ongoing.");
        break;
    }
    default:
    {
        LOG_MODEL_INFO("CFrmSendFile","The transfer is finished");
//        EpPublic* p = EpPublic::instance();
//        emit sigImgRecieved(p->getReceiveDefaultSaveDir().absolutePath() + QDir::separator() +m_job.fileName());
//        emit sigImgRecieved(m_job.fileName());
        this->close();
        break;
    }
    }
}

void CFrmSendFile::onLocalFileUrlChanged(const QUrl &localFileUrl)
{
    LOG_MODEL_DEBUG("CFrmSendFile","onLocalFileUrlChanged");
    Q_UNUSED(localFileUrl);
}

void CFrmSendFile::onSetGauge(const QString &gauge, double value)
{
    LOG_MODEL_DEBUG("CFrmSendFile","onSetGauge");
    Q_UNUSED(gauge);
    Q_UNUSED(value);
}

void CFrmSendFile::onLogMessage(QXmppLogger::MessageType type, const QString &msg)
{
    LOG_MODEL_DEBUG("CFrmSendFile","onLogMessage");
    Q_UNUSED(type);
    Q_UNUSED(msg);
}

void CFrmSendFile::onUpdateCounter(const QString &counter, qint64 amount)
{
    LOG_MODEL_DEBUG("CFrmSendFile","onUpdateCounter");
    Q_UNUSED(counter);
    Q_UNUSED(amount);
}

void CFrmSendFile::on_btn_accept_clicked()
{
    ui->progressBar->setHidden(false);
    ui->btn_accept->setEnabled(false);
    m_job.accept(m_saveFilePath);
}

void CFrmSendFile::on_btn_saveAs_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               m_saveFilePath,
                               tr("All (*.*)"));
    if(fileName.isEmpty())
    {
        return;
    }
    m_saveFilePath = fileName;
    on_btn_accept_clicked();
}

void CFrmSendFile::on_btn_abort_clicked()
{
    ui->progressBar->setHidden(true);
    ui->btn_abort->setEnabled(false);
    m_job.abort();
    this->close();
}
