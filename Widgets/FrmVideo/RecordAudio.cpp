#include "RecordAudio.h"
#include <QDebug>

CRecordAudio::CRecordAudio(QIODevice *pChannel, QString szRecordFile, QObject *parent) :
    QIODevice(parent),
    m_pChannel(pChannel),
    m_RecordFile(szRecordFile)
{
}

bool CRecordAudio::open(OpenMode mode)
{
    if(!m_RecordFile.fileName().isEmpty())
    {
        if(!m_RecordFile.open(mode))
            qDebug() << "fail:CRecordAudio::open file:" << m_RecordFile.fileName();
    }
    return QIODevice::open(mode);
}

void CRecordAudio::close()
{
    if(m_RecordFile.isOpen())
        m_RecordFile.close();
    return QIODevice::close();
}

qint64 CRecordAudio::readData(char *data, qint64 maxlen)
{
    int nRet = 0;
    qDebug() << "CRecordAudio::readData";

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
    qDebug() << "CRecordAudio::writeData";
    if(!m_pChannel)
        return -1;

    nRet = m_pChannel->write(data, len);

    if(m_RecordFile.isOpen())
        m_RecordFile.write(data, len);

    return nRet;
}
