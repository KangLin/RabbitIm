#ifndef UPDATE_H
#define UPDATE_H

#ifndef RABBITIM_USER_LIBCURL
#error "Don't RABBITIM_USER_LIBCURL=1";
#endif

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
