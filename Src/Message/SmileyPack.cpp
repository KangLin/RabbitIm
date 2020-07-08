
#include "SmileyPack.h"

#include <QFileInfo>
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>
#include <QDomDocument>
#include <QDomElement>
#include <QBuffer>
#include <QStringBuilder>
#include "Global/Global.h"

CSmileyPack::CSmileyPack()
{
    load(CGlobalDir::Instance()->GetFileSmileyPack());
}

CSmileyPack& CSmileyPack::getInstance()
{
    static CSmileyPack smileyPack;
    return smileyPack;
}

QList<QPair<QString, QString> > CSmileyPack::listSmileyPacks(QStringList &paths)
{
    if(paths.isEmpty())
    {
        paths << "./smileys" << "/usr/share/qtox/smileys" << "/usr/share/emoticons" << "~/.kde4/share/emoticons" << "~/.kde/share/emoticons";
    }
    QList<QPair<QString, QString> > smileyPacks;

    for (QString path : paths)
    {
        if (path.leftRef(1) == "~")
            path.replace(0, 1, QDir::homePath());

        QDir dir(path);
        if (!dir.exists())
            continue;

        for (const QString& subdirectory : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            dir.cd(subdirectory);

            QFileInfoList entries = dir.entryInfoList(QStringList() << "emoticons.xml", QDir::Files);
            if (entries.size() > 0) // does it contain a file called emoticons.xml?
            {
                QString packageName = dir.dirName();
                QString absPath = entries[0].absoluteFilePath();
                QString relPath = QDir(QCoreApplication::applicationDirPath()).relativeFilePath(absPath);

                if (relPath.leftRef(2) == "..")
                    smileyPacks << QPair<QString, QString>(packageName, absPath);
                else
                    smileyPacks << QPair<QString, QString>(packageName, relPath); // use relative path for subdirectories
            }

            dir.cdUp();
        }
    }

    return smileyPacks;
}

bool CSmileyPack::isValid(const QString &filename)
{
    return QFile(filename).exists();
}

bool CSmileyPack::load(const QString& filename)
{
    // discard old data
    filenameTable.clear();
    imgCache.clear();
    emoticons.clear();
    path.clear();

    // open emoticons.xml
    QFile xmlFile(filename);
    if(!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("CSmileyPack", "Don't open file:%s", qPrintable(filename));
        return false; // cannot open file
    }
    /* parse the cfg file
     * sample:
     * <?xml version='1.0'?>
     * <messaging-emoticon-map>
     *   <emoticon file="smile.png" >
     *       <string>:)</string>
     *       <string>:-)</string>
     *   </emoticon>
     *   <emoticon file="sad.png" >
     *       <string>:(</string>
     *       <string>:-(</string>
     *   </emoticon>
     * </messaging-emoticon-map>
     */

    path = QFileInfo(filename).absolutePath();

    QDomDocument doc;
    doc.setContent(xmlFile.readAll());

    QDomNodeList emoticonElements = doc.elementsByTagName("emoticon");
    for (int i = 0; i < emoticonElements.size(); ++i)
    {
        QString file = emoticonElements.at(i).attributes().namedItem("file").nodeValue();
        QDomElement stringElement = emoticonElements.at(i).firstChildElement("string");

        QStringList emoticonSet; // { ":)", ":-)" } etc.

        while (!stringElement.isNull())
        {
            QString emoticon = stringElement.text();
            filenameTable.insert(emoticon, file);
            emoticonSet.push_back(emoticon);
            cacheSmiley(file); // preload all smileys

            stringElement = stringElement.nextSibling().toElement();
        }
        emoticons.push_back(emoticonSet);
    }

    // success!
    return true;
}

QString CSmileyPack::smileyfied(QString msg)
{
    QRegExp exp("\\S+"); // matches words

    int index = msg.indexOf(exp);

    // if a word is key of a smiley, replace it by its corresponding image in Rich Text
    while (index >= 0)
    {
        QString key = exp.cap();
        if (filenameTable.contains(key))
        {
            QString imgRichText = getAsRichText(key);

            msg.replace(index, key.length(), imgRichText);
            index += imgRichText.length() - key.length();
        }
        index = msg.indexOf(exp, index + key.length());
    }

    return msg;
}

QList<QStringList> CSmileyPack::getEmoticons() const
{
    return emoticons;
}

QString CSmileyPack::getAsRichText(const QString &key)
{
    return "<img src=\"data:image/png;base64," % QString(getCachedSmiley(key).toBase64()) % "\">";
}

QIcon CSmileyPack::getAsIcon(const QString &key)
{
    QPixmap pm;
    pm.loadFromData(getCachedSmiley(key), "PNG");

    return QIcon(pm);
}

void CSmileyPack::cacheSmiley(const QString &name)
{
    QSize size(16, 16); // TODO: adapt to text size
    QString filename = QDir(path).filePath(name);
    QImage img(filename);

    if (!img.isNull())
    {
        QImage scaledImg = img.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QByteArray scaledImgData;
        QBuffer buffer(&scaledImgData);
        scaledImg.save(&buffer, "PNG");

        imgCache.insert(name, scaledImgData);
    }
}

QByteArray CSmileyPack::getCachedSmiley(const QString &key)
{
    // valid key?
    if (!filenameTable.contains(key))
        return QByteArray();

    // cache it if needed
    QString file = filenameTable.value(key);
    if (!imgCache.contains(file)) {
        cacheSmiley(file);
    }

    return imgCache.value(file);
}

void CSmileyPack::onSmileyPackChanged()
{
    load(CGlobalDir::Instance()->GetFileSmileyPack());
}
