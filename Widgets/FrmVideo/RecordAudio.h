#ifndef RECORDAUDIO_H
#define RECORDAUDIO_H

#include <QIODevice>
#include <QFile>

class CRecordAudio : public QIODevice
{
    Q_OBJECT
public:
    explicit CRecordAudio(QIODevice *pChannel, QString szRecordFile = QString(), QObject *parent = 0);

    virtual bool open(OpenMode mode);
    virtual void close();

signals:

public slots:

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private:
    QIODevice* m_pChannel;
    QFile m_RecordFile;
};

#endif // RECORDAUDIO_H
