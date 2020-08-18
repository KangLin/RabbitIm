#include "PluginConverFormat.h"

CConverFormat::CConverFormat(QObject *parent) : QObject(parent)
{}

int CConverFormat::Initialize()
{
    return 0;
}

int CConverFormat::Clean()
{
    return 0;
}

QString CConverFormat::getName()
{
    return this->metaObject()->className();
}
