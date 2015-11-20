#include "PluginApp.h"
#include <QMessageBox>
#include "Global/Log.h"

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
