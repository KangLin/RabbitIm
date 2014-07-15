#ifndef ZFILEPROGRESS_H
#define ZFILEPROGRESS_H
#include "qxmpp/QXmppTransferManager.h"
#include <QFrame>

namespace Ui {
class CFrmSendFile;
}

class CFrmSendFile : public QFrame
{
    Q_OBJECT

public:
    CFrmSendFile(QXmppTransferJob& job,bool isSend = false,QWidget* parent = 0);
    ~CFrmSendFile();
    void setId(const QString& id);

protected:
    void closeEvent(QCloseEvent *);

private:
    void initConnect();

public slots:
    void onFileReceiveProgress(qint64 done, qint64 total);
    void onFinished();
    void onError(QXmppTransferJob::Error error);
    void onStateChanged(QXmppTransferJob::State state);
    //===================================
    void onLocalFileUrlChanged(const QUrl &localFileUrl);
    void onSetGauge(const QString &gauge, double value);
    void onLogMessage(QXmppLogger::MessageType type, const QString &msg);
    void onUpdateCounter(const QString &counter, qint64 amount = 1);


private slots:
    void on_btn_accept_clicked();
    void on_btn_saveAs_clicked();
    void on_btn_abort_clicked();

signals:
    void sigImgRecieved(const QString& fileName);
    void sigClosed();

private:
    enum E_STATUS_TYPE{E_RECIEVE,E_CANCEL};
    Ui::CFrmSendFile* ui;
    E_STATUS_TYPE m_type;
    QString m_id;
    QXmppTransferJob& m_job;
    bool m_isImg;
    QString m_saveFilePath;
};

#endif // ZFILEPROGRESS_H
