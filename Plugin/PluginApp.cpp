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
    qDebug("CPluginApp::~CPluginApp");
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
#if defined (_DEBUG)
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
    qDebug("locale language:%s; Translate dir:%s; dir:%s",
                    szLocale.toStdString().c_str(), qPrintable(szPlugin),
                    szDir.toStdString().c_str());
    bool bRet = m_TranslatorPlugin->load(szPlugin);
    if(!bRet)
        qCritical("load translator[%s] fail.Dir: %s",
                        szPlugin.toStdString().c_str(),
                        szDir.toStdString().c_str());
    
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
