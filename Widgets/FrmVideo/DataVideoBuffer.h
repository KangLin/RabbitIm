/* 视频缓存类，用于从一个buffer构造QVideoFrame
 * 作者：康林（msn、email：kl222@126.com）
 */

#ifndef DATAVIDEOBUFFER_H
#define DATAVIDEOBUFFER_H

#include <QAbstractVideoBuffer>

class CDataVideoBuffer : public QAbstractVideoBuffer
{
public:
    CDataVideoBuffer(uchar* data, int nLength, int nWidth, int nHeight);

    virtual MapMode mapMode() const;

    virtual uchar *map(MapMode mode, int *numBytes, int *bytesPerLine);
    virtual void unmap();

private:
    uchar* m_pData;
    int m_nLength;
    MapMode m_Mode;
    int m_nWidth;
    int m_nHeight;

};

#endif // DATAVIDEOBUFFER_H
