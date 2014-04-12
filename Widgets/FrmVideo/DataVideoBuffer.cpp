#include "DataVideoBuffer.h"

CDataVideoBuffer::CDataVideoBuffer(uchar* data, int nLength, int nWidth, int nHeight)
     : QAbstractVideoBuffer(NoHandle)
      ,m_Mode(NotMapped)
{
    m_pData = data;
    m_nLength = nLength;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
}

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
        *numBytes = m_nLength;

    if (bytesPerLine)
        *bytesPerLine = m_nLength / m_nHeight;

    return m_pData;
}

void CDataVideoBuffer::unmap()
{
    m_Mode = NotMapped;
}
