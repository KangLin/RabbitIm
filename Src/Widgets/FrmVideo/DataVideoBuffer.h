/* 视频缓存类，用于从一个buffer构造QVideoFrame
 * 作者：康林（msn、email：kl222@126.com）
 */

#ifndef DATAVIDEOBUFFER_H
#define DATAVIDEOBUFFER_H

#include <QAbstractVideoBuffer>
#include "Global/GlobalDir.h"

class RABBITIM_SHARED_LIBRARY CDataVideoBuffer : public QAbstractVideoBuffer
{
public:
    CDataVideoBuffer(const QByteArray &data, int nWidth, int nHeight);
    virtual ~CDataVideoBuffer();
    virtual MapMode mapMode() const;

    virtual uchar *map(MapMode mode, int *numBytes, int *bytesPerLine);
    virtual void unmap();

private:
    QByteArray m_Data;
    MapMode m_Mode;
    int m_nWidth;
    int m_nHeight;

};

#endif // DATAVIDEOBUFFER_H
