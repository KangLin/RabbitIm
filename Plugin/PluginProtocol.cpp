#include "PluginProtocol.h"
#include "Global/Global.h"
#include <QApplication>
#include <QSettings>
#include <QSharedPointer>
#include "RabbitCommonDir.h"

CPluginProtocol::CPluginProtocol()
{
}

/**
 * @brief CPluginProtocol::InitInstance
 * @param szDir
 * @return 
 * @see CManager
 */
int CPluginProtocol::InitInstance(const QString &szDir)
{
    int nRet = 0;
    nRet = LoadTranslate(szDir);
    return nRet;
}

int CPluginProtocol::ClearInstance()
{
    if(!m_TranslatorPlugin.isNull())
    {
        qApp->removeTranslator(m_TranslatorPlugin.data());
        m_TranslatorPlugin.clear();
    }
    return 0;
}

int CPluginProtocol::LoadTranslate(const QString &szDir)
{
    Q_UNUSED(szDir)
    //初始化翻译
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", QLocale::system().name()).toString();

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    LOG_MODEL_DEBUG("CPluginApp", "locale language:%s", szLocale.toStdString().c_str());

    if(!m_TranslatorPlugin.isNull())
    {
        qApp->removeTranslator(m_TranslatorPlugin.data());
        m_TranslatorPlugin.clear();
    }

    m_TranslatorPlugin = QSharedPointer<QTranslator>(new QTranslator());

    QString szPlugin;
#if defined (DEBUG)
    szPlugin = ":/translations/" + ID() + "_" + szLocale + ".qm";
#elif ANDROID
    szPlugin = QString("assets:/plugins/translations")
           + QDir::separator() + ID() + "_" + szLocale + ".qm";
#else
    if(szDir.isEmpty())
        szPlugin = ":/translations/" + ID() + "_" + szLocale + ".qm";
    else
        szPlugin = szDir + QDir::separator() + "translations"
               + QDir::separator() + ID() + "_" + szLocale + ".qm";
#endif

    LOG_MODEL_DEBUG("CPluginApp", "Translate dir:%s", qPrintable(szPlugin));
    bool bRet = m_TranslatorPlugin->load(szPlugin);
    if(!bRet)
        LOG_MODEL_ERROR("CPluginApp", "load translator[%s] fail",
                        szPlugin.toStdString().c_str());

    qApp->installTranslator(m_TranslatorPlugin.data());
    return 0;
}
