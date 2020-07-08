#include "style.h"
#include "Global/Global.h"
#include <QFile>
#include <QDebug>

QString CStyle::get(const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text))
        return file.readAll();
    else
        LOG_MODEL_WARNING("CStyle", "file not found:%s", qPrintable(filename));
    return QString();
}
