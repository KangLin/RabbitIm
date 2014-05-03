#ifndef RECORDAUDIO_H
#define RECORDAUDIO_H

#include <QIODevice>
#include <QFile>

class CRecordAudio : public QIODevice
{
    Q_OBJECT
public:
    explicit CRecordAudio(QObject *parent = 0);

    virtual bool open(OpenMode mode, QIODevice* pChannel, QIODevice *pOutDevice = NULL, QString szRecordFile = QString());
    virtual void close();

signals:

private slots:
    void slotReadyRead();

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    QIODevice* m_pChannel, *m_pOutDevice;
    QFile m_RecordFile;
};

#endif // RECORDAUDIO_H
