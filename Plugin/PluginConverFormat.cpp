#include "PluginConverFormat.h"

CPluginConverFormat::CPluginConverFormat(QObject *parent) : QObject(parent)
{}

int CPluginConverFormat::Initialize()
{
    return 0;
}

int CPluginConverFormat::Clean()
{
    return 0;
}

QString CPluginConverFormat::getName()
{
    return this->metaObject()->className();
}

int CPluginConverFormat::getPriority()
{
    return 0;
}
