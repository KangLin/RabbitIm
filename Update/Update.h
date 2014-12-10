#ifndef UPDATE_H
#define UPDATE_H

#include "DownLoadHandle.h"
#include <string>
#include <QString>

class CUpdate
{
public:
    CUpdate();
    int Run();

private:
    CDownLoad m_DownLoad;
    CDownLoadHandleVersionFile m_VersionFileHandle;

    QString m_szUrl, m_szFile;
};

#endif // UPDATE_H
