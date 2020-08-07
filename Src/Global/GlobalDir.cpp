#include "GlobalDir.h"
#include "Log.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include "RabbitCommonDir.h"

#ifdef RABBITIM
    #include "Global.h"
#endif
#ifdef RABBITIM_USE_QXMPP
    #include "QXmppUtils.h"
#endif

CGlobalDir::CGlobalDir()
{}

CGlobalDir* CGlobalDir::Instance()
{
    static CGlobalDir* p = nullptr;
    if(!p)
        p = new CGlobalDir;
    return p;
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
        szDir = RabbitCommon::CDir::Instance()->GetDirUserDocument()
                + QDir::separator() + "Motion";
    else
        szDir = GetDirUserData(szKey) + QDir::separator() + "Motion";

    QDir d;
    if(!d.exists(szDir))
        d.mkdir(szDir);
    return szDir;
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
    QString path = RabbitCommon::CDir::Instance()->GetDirUserDocument()
            + QDir::separator()
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
    QString path = RabbitCommon::CDir::Instance()->GetDirUserDocument()
            + QDir::separator() + jid
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

QString CGlobalDir::GetUserConfigureFile(const QString &szId)
{
    return GetDirUserConfigure(szId) + QDir::separator() + "user.conf";
}

QString CGlobalDir::GetDirEmoji()
{
    return RabbitCommon::CDir::Instance()->GetDirApplicationInstallRoot()
            + QDir::separator() + "emoji";
}
