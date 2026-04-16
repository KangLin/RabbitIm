#include "PluginConvertFormat.h"

CPluginConvertFormat::CPluginConvertFormat(QObject *parent) : QObject(parent)
{}

int CPluginConvertFormat::Initialize()
{
    return 0;
}

int CPluginConvertFormat::Clean()
{
    return 0;
}

QString CPluginConvertFormat::getName()
{
    return this->metaObject()->className();
}

int CPluginConvertFormat::getPriority()
{
    return 0;
}
