#include "Global.h"

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
}

CGlobal::~CGlobal()
{
}

CGlobal g_Global;
