#include "PluginApp.h"
#include <QMessageBox>
#include "Global/Log.h"

CPluginApp::CPluginApp(QObject *parent) : QObject(parent)
{  
}

CPluginApp::~CPluginApp()
{
    LOG_MODEL_DEBUG("CPluginApp", "CPluginApp::~CPluginApp");
}

int CPluginApp::About()
{
    QMessageBox::information(NULL, tr("Application Infomation"),
                             tr("Application:") + Name());
    return 0;
}

QIcon CPluginApp::Icon()
{
    return QIcon(":/icon/AppIcon");
}
