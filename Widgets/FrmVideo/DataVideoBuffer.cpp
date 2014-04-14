#include "DataVideoBuffer.h"

CDataVideoBuffer::CDataVideoBuffer(const QByteArray &data, int nWidth, int nHeight)
     : QAbstractVideoBuffer(NoHandle)
      ,m_Data(data)
      ,m_Mode(NotMapped)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
}

CDataVideoBuffer::~CDataVideoBuffer()
{}

QAbstractVideoBuffer::MapMode CDataVideoBuffer::mapMode() const
{
    return m_Mode;
}

uchar* CDataVideoBuffer::map(MapMode mode, int *numBytes, int *bytesPerLine)
{
    if (m_Mode != NotMapped)
        return 0;

    m_Mode = mode;

    if (numBytes)
        *numBytes = m_Data.size();

    if (bytesPerLine)
        *bytesPerLine =  m_Data.size() / m_nHeight;

    return reinterpret_cast<uchar *>(m_Data.data());
}

void CDataVideoBuffer::unmap()
{
    m_Mode = NotMapped;
}
