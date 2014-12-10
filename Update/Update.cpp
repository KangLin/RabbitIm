#include "Update.h"
#include "Global/Global.h"
#include <QDir>
#include "Version.h"

#ifndef RABBITIM_USER_LIBCURL
#error "Don't RABBITIM_USER_LIBCURL=1";
#endif

CUpdate::CUpdate()
{
    QString szFile = "Update_";
    szFile += RABBITIM_SYSTEM;
    szFile += RABBITIM_PLATFORM;
    szFile += RABBITIM_ARCHITECTURE;
    szFile += ".xml";
    m_szFile = CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + szFile;
    //m_szUrl = 
}

int CUpdate::Run()
{
    m_VersionFileHandle.SetFile(m_szFile.toStdString());
    m_DownLoad.Start(m_szUrl.toStdString(), 
                     m_szFile.toStdString(), 
                     &m_VersionFileHandle, 
                     1);
    return 0;
}
