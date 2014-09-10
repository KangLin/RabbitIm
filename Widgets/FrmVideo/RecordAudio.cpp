#include "RecordAudio.h"
#include <QDebug>
#include <QTime>
#include <QThread>
#include "../../Global/Global.h"

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
            LOG_MODEL_ERROR("RecordAudio", "fail:CRecordAudio::open file:%s\n" , qPrintable(m_RecordFile.fileName()));
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
#ifdef DEBUG_VIDEO_TIME
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("RecordAudio", "CRecordAudio::slotReadyRead:threadid:%d, preTime:%s, currTime:%s, space:%d",
           QThread::currentThreadId(),
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
    if(!m_pChannel || !m_pOutDevice)
    {
        return;
    }

    qint64 size = m_pChannel->bytesAvailable();
    
    //LOG_MODEL_DEBUG("RecordAudio", "bytesAvailable:%d", size);
    if(0 >= size)
    {
        LOG_MODEL_ERROR("RecordAudio", "bytesAvailable is 0");
        return;
    }

    char* pBuf = new char[size];
    if(NULL == pBuf)
    {
        LOG_MODEL_ERROR("RecordAudio", "don't has memory");
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
       LOG_MODEL_DEBUG("RecordAudio", "CRecordAudio::slotReadyRead is 0");

    delete[] pBuf;
}

qint64 CRecordAudio::readData(char *data, qint64 maxlen)
{
    int nRet = 0;
#ifdef DEBUG_VIDEO_TIME
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("RecordAudio", "CRecordAudio::readData:threadid:%d, preTime:%s, currTime:%s, space:%d",
           QThread::currentThreadId(),
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
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
#ifdef DEBUG_VIDEO_TIME
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("RecordAudio", "CRecordAudio::writeData:threadid:%d, preTime:%s, currTime:%s, space:%d",
           QThread::currentThreadId(),
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
    if(!m_pChannel)
        return -1;

    nRet = m_pChannel->write(data, len);

    if(m_RecordFile.isOpen())
        m_RecordFile.write(data, len);

    return nRet;
}
