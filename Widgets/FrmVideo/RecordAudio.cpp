#include "RecordAudio.h"
#include <QDebug>
#include <QTime>
#include <QThread>

CRecordAudio::CRecordAudio(QObject *parent) :
    QIODevice(parent),
    m_pChannel(NULL),
    m_pOutDevice(NULL)
{
}

bool CRecordAudio::open(OpenMode mode, QIODevice *pChannel, QIODevice *pOutDevice, QString szRecordFile)
{
    m_RecordFile.setFileName(szRecordFile);
    m_pChannel = pChannel;
    m_pOutDevice = pOutDevice;

    if(!m_RecordFile.fileName().isEmpty())
    {
        if(!m_RecordFile.open(mode))
            qDebug() << "fail:CRecordAudio::open file:" << m_RecordFile.fileName();
    }

    if(m_pChannel && m_pOutDevice)
    {
        bool check = connect(m_pChannel, SIGNAL(readyRead()),
                             SLOT(slotReadyRead()));
        Q_ASSERT(check);
    }
    return QIODevice::open(mode);
}

void CRecordAudio::close()
{
    if(m_RecordFile.isOpen())
        m_RecordFile.close();

    if(m_pChannel && m_pOutDevice)
    {
        m_pChannel->disconnect(this);
    }

    m_RecordFile.setFileName("");
    m_pChannel = NULL;
    m_pOutDevice = NULL;

    return QIODevice::close();
}

void CRecordAudio::slotReadyRead()
{
    qDebug() << "CRecordAudio::slotReadyRead threadid:" << QThread::currentThreadId()
             << "CRecordAudio::slotReadyRead" << QTime::currentTime().toString("hh:mm:ss.zzz");
    if(!m_pChannel || !m_pOutDevice)
    {
        return;
    }

    qint64 size = m_pChannel->bytesAvailable();
    qDebug() << "bytesAvailable:" << size;
    if(0 >= size)
    {
        qDebug() << "bytesAvailable is 0";
        return;
    }

    char* pBuf = new char[size];
    if(NULL == pBuf)
    {
        qDebug() << "don't has memory";
        return;
    }

    int nLen = m_pChannel->read(pBuf, size);
    if(nLen > 0)
    {
        m_pOutDevice->write(pBuf, nLen);

        if(m_RecordFile.isOpen())
            m_RecordFile.write(pBuf, nLen);
    }
    else
        qDebug() << "CRecordAudio::slotReadyRead is 0";

    delete[] pBuf;
}

qint64 CRecordAudio::readData(char *data, qint64 maxlen)
{
    int nRet = 0;
    qDebug()  << "thread:" << QThread::currentThreadId()
              << "CRecordAudio::readData" << QTime::currentTime().toString("hh:mm:ss.zzz");

    if(!m_pChannel)
        return -1;

    nRet = m_pChannel->read(data, maxlen);

    if(m_RecordFile.isOpen())
        m_RecordFile.write(data, nRet);

    return nRet;
}

qint64 CRecordAudio::writeData(const char *data, qint64 len)
{
    int nRet = 0;
    qDebug() << "thread:" << QThread::currentThreadId()
             << "CRecordAudio::writeData:len:" << len
             << ";data:" << QTime::currentTime().toString("hh:mm:ss.zzz");
    if(!m_pChannel)
        return -1;

    nRet = m_pChannel->write(data, len);

    if(m_RecordFile.isOpen())
        m_RecordFile.write(data, len);

    return nRet;
}
