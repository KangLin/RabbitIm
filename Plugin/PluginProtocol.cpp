#include "PluginProtocol.h"
#include "Global/Global.h"
#include <QApplication>
#include <QSettings>
#include <QSharedPointer>

CPluginProtocol::CPluginProtocol()
{
}

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
    //初始化翻译
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
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
#ifdef ANDROID
    szPlugin = ":/translations/Plugin_" + szLocale + ".qm";
#else
    if(szDir.isEmpty())
    {
        szPlugin = ":/translations/Plugin_" + szLocale + ".qm";
    }
    else
    {
        szPlugin = szDir + QDir::separator() + "translations"
                + QDir::separator() + "Plugin_" + szLocale + ".qm";
    }
#endif
    LOG_MODEL_DEBUG("CPluginApp", "Translate dir:%s", qPrintable(szPlugin));
    bool bRet = m_TranslatorPlugin->load(szPlugin);
    if(!bRet)
        LOG_MODEL_ERROR("CPluginApp", "load translator[%s] fail",
                        szPlugin.toStdString().c_str());

    qApp->installTranslator(m_TranslatorPlugin.data());
    return 0;
}
