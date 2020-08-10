#include "PluginApp.h"
#include "Global/Global.h"
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QSharedPointer>
#include "RabbitCommonDir.h"

CPluginApp::CPluginApp()
{  
}

CPluginApp::~CPluginApp()
{
    LOG_MODEL_DEBUG("CPluginApp", "CPluginApp::~CPluginApp");
}

int CPluginApp::About()
{
    QMessageBox::information(NULL, QObject::tr("Application Infomation"),
                             QObject::tr("Application:") + Name());
    return 0;
}

QIcon CPluginApp::Icon()
{
    return QIcon(":/icon/AppIcon");
}

int CPluginApp::InitInstance(const QString &szDir)
{
    int nRet = 0;
    nRet = LoadTranslate(szDir);
    return nRet;
}

int CPluginApp::ClearInstance()
{
    ClearTranslate();
    return 0;
}

int CPluginApp::LoadTranslate(const QString &szDir)
{
    Q_UNUSED(szDir)
    //初始化翻译  
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", QLocale::system().name()).toString();

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    ClearTranslate();

    m_TranslatorPlugin = QSharedPointer<QTranslator>(new QTranslator());

    QString szPlugin;
#if defined (ANDROID) || defined (DEBUG)
    szPlugin = ":/translations/Plugin_" + szLocale + ".qm";
#else
    if(szDir.isEmpty())
        szPlugin = ":/translations/Plugin_" + szLocale + ".qm";
    else
        szPlugin = szDir + QDir::separator() + "translations"
               + QDir::separator() + "Plugin_" + szLocale + ".qm";
#endif
    LOG_MODEL_DEBUG("CPluginApp", "locale language:%s; Translate dir:%s",
                    szLocale.toStdString().c_str(), qPrintable(szPlugin));
    bool bRet = m_TranslatorPlugin->load(szPlugin);
    if(!bRet)
        LOG_MODEL_ERROR("CPluginApp", "load translator[%s] fail",
                        szPlugin.toStdString().c_str());
    
    qApp->installTranslator(m_TranslatorPlugin.data());
    return 0;
}

int CPluginApp::ClearTranslate()
{
    if(!m_TranslatorPlugin.isNull())
    {
        qApp->removeTranslator(m_TranslatorPlugin.data());
        m_TranslatorPlugin.clear();
    }
    return 0;
}
