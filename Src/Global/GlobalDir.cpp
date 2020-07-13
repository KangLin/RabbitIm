#include "GlobalDir.h"
#include "Log.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>

#ifdef RABBITIM
    #include "Global.h"
#endif
#ifdef RABBITIM_USE_QXMPP
    #include "QXmppUtils.h"
#endif

CGlobalDir::CGlobalDir()
{
    //注意这个必须的在最前  
    m_szDocumentPath =  QStandardPaths::writableLocation(
                QStandardPaths::DocumentsLocation);
    if(m_szDocumentPath.isEmpty())
    {
        LOG_MODEL_ERROR("CGlobal", "document path is empty");
    }
}

CGlobalDir* CGlobalDir::Instance()
{
    static CGlobalDir* p = NULL;
    if(!p)
        p = new CGlobalDir;
    return p;
}

QString CGlobalDir::GetDirApplication()
{
#ifdef ANDROID
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath() + QDir::separator() + "..";
#else
    //LOG_MODEL_DEBUG("global", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath();
#endif
}

QString CGlobalDir::GetDirDocument()
{
    QString szPath;
    if(m_szDocumentPath.right(1) == "/"
            || m_szDocumentPath.right(1) == "\\")
        szPath = m_szDocumentPath.left(m_szDocumentPath.size() - 1);
    else
        szPath = m_szDocumentPath;
    szPath += QDir::separator();
    szPath = szPath + "Rabbit"
#ifdef RABBITIM
             + QDir::separator() + "Im"
#endif
            ;
    return szPath;
}

int CGlobalDir::SetDirDocument(QString szPath)
{
    m_szDocumentPath = szPath + QDir::separator() + "Rabbit"
#ifdef RABBITIM
            + QDir::separator() + "Im"
#endif
            ;
    return 0;
}

/*
QString CGlobalDir::GetDirApplicationConfigure()
{
    return GetDirDocument() + QDir::separator() + "conf";
}

QString CGlobalDir::GetDirApplicationData()
{
    return GetDirDocument() + QDir::separator() + "Data";
}
*/

QString CGlobalDir::GetDirApplicationDownLoad()
{
    QString szDownLoad = GetDirDocument() + QDir::separator() + "DownLoad";
    QDir d;
    if(!d.exists(szDownLoad))
        d.mkdir(szDownLoad);
    return szDownLoad;
}

QString CGlobalDir::GetDirMotion(const QString &szId)
{
    QString szKey = szId;
#ifdef RABBITIM
    if(szId.isEmpty())
    {
        szKey = USER_INFO_LOCALE->GetInfo()->GetId();
    }
#endif
    QString szDir;
    if(szKey.isEmpty())
        szDir = GetDirDocument() + QDir::separator() + "Motion";
    else
        szDir = GetDirUserData(szKey) + QDir::separator() + "Motion";

    QDir d;
    if(!d.exists(szDir))
        d.mkdir(szDir);
    return szDir;
}

//应用程序的配置文件  
QString CGlobalDir::GetApplicationConfigureFile()
{
    return GetDirDocument() + QDir::separator() + "app.conf";
}

QString CGlobalDir::GetDirTranslate()
{
#ifdef ANDROID
    //TODO:android下应该在安装包中装好语言  
    return GetDirDocument() + QDir::separator() + "translations";
#endif
    return GetDirApplication() + QDir::separator() + "translations";
}

QString CGlobalDir::GetDirUserConfigure(const QString &szId)
{
    QString jid;
    if(szId.isEmpty())
    {
#ifdef RABBITIM
        if(!GLOBAL_USER.isNull()
                && !USER_INFO_LOCALE.isNull())
        {
            jid = USER_INFO_LOCALE->GetInfo()->GetId();
        }
        else
        {
            LOG_MODEL_ERROR("Global", "Don't initialization GetGlobalUser or GetUserInfoLocale");
            Q_ASSERT(false);
        }
#else
        ;
#endif
    }
    else
        jid = szId;
    jid = jid.replace("@", ".");
    QString path = GetDirDocument() + QDir::separator()
            + jid + QDir::separator() + "conf";
    QDir d;
    if(!d.exists(path))
    {
        if(!d.mkpath(path))
            return QString();
    }
    return path;
}

QString CGlobalDir::GetDirUserData(const QString &szId)
{
    QString jid;
    if(szId.isEmpty())
    {
#ifdef RABBITIM
        if(!GLOBAL_USER.isNull()
                && !USER_INFO_LOCALE.isNull())
        {
            jid = USER_INFO_LOCALE->GetInfo()->GetId();
        }
        else
        {
            LOG_MODEL_ERROR("Global", "Don't initialization GetGlobalUser or GetUserInfoLocale");
            Q_ASSERT(false);
        }
#else
        ;
#endif
    }
    else
        jid = szId;
    jid = jid.replace("@", ".");
    QString path = GetDirDocument() + QDir::separator() + jid
            + QDir::separator() + "data";
    QDir d;
    if(!d.exists(path))
    {
        if(!d.mkpath(path))
            return QString();
    }
    return path;
}

QString CGlobalDir::GetDirTempDir(const QString &szId)
{
    QString dirHeads = GetDirUserData(szId) + QDir::separator() + "Temp";
    QDir d;
    if(!d.exists(dirHeads))
        if(!d.mkdir(dirHeads))
            LOG_MODEL_ERROR("CGlobal", "mkdir GetDirTempDir error:%s", qPrintable(dirHeads));
    return dirHeads;
}

QString CGlobalDir::GetDirUserAvatar(const QString &szId)
{
    QString dirHeads = GetDirUserData(szId) + QDir::separator() + "Avatars";
    QDir d;
    if(!d.exists(dirHeads))
        if(!d.mkdir(dirHeads))
            LOG_MODEL_ERROR("CGlobal", "mkdir GetUserDataAvatar error:%s", qPrintable(dirHeads));
    return dirHeads;
}

QString CGlobalDir::GetDirReceiveFile(const QString &szId)
{
    QString dir = GetDirUserData(szId) + QDir::separator() + "ReceiveFiles";
    QDir d;
    if(!d.exists(dir))
        if(!d.mkdir(dir))
            LOG_MODEL_ERROR("CGlobal", "mkdir GetUserDataAvatar error:%s", qPrintable(dir));
    return dir;
}

QString CGlobalDir::GetFileUserAvatar(const QString &szId, const QString &szLocalId)
{
    QString id = szId;
#ifdef RABBITIM_USE_QXMPP
    if(!szId.isEmpty())
        id = QXmppUtils::jidToBareJid(id);
#endif
    id = id.replace("@", ".");

    return GetDirUserAvatar(szLocalId) + QDir::separator() + id + ".png";
}

QString CGlobalDir::GetFileSmileyPack()
{
    return QString(":/smileys/default/emoticons.xml");
}

QString CGlobalDir::GetUserConfigureFile(const QString &szId)
{
    return GetDirUserConfigure(szId) + QDir::separator() + "user.conf";
}
