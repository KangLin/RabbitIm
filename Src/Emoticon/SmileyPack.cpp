#include "SmileyPack.h"
#include "Global/Global.h"
#include <QDir>
#include <QDomElement>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QStringBuilder>
#include <QtConcurrent/QtConcurrentRun>
#include <QTimer>

#if defined(Q_OS_FREEBSD)
#include <locale.h>
#endif

/**
 * @class CSmileyPack
 * @brief Maps emoticons to smileys.
 *
 * @var CSmileyPack::filenameTable
 * @brief Matches an emoticon to its corresponding smiley ie. ":)" -> "happy.png"
 *
 * @var CSmileyPack::iconCache
 * @brief representation of a smiley ie. "happy.png" -> data
 *
 * @var CSmileyPack::emoticons
 * @brief {{ ":)", ":-)" }, {":(", ...}, ... }
 *
 * @var CSmileyPack::path
 * @brief directory containing the cfg and image files
 *
 * @var CSmileyPack::defaultPaths
 * @brief Contains all directories where smileys could be found
 */

QStringList loadDefaultPaths();

static const QStringList DEFAULT_PATHS = loadDefaultPaths();

static const QString RICH_TEXT_PATTERN = QStringLiteral("<img title=\"%1\" alt=\"%1\" src=\"key:%1\"/>");

static const QString EMOTICONS_FILE_NAME = QStringLiteral("emoticons.xml");

static constexpr int CLEANUP_TIMEOUT = 5 * 60 * 1000; // 5 minutes

/**
 * @brief Construct list of standard directories with "emoticons" sub dir, whether these directories
 * exist or not
 * @return Constructed list of default emoticons directories
 */
QStringList loadDefaultPaths()
{
#if defined(Q_OS_FREEBSD)
    // TODO: Remove when will be fixed.
    // Workaround to fix https://bugreports.qt.io/browse/QTBUG-57522
    setlocale(LC_ALL, "");
#endif
    const QString EMOTICONS_SUB_PATH = QDir::separator() + QStringLiteral("emoticons");
    QStringList paths{":/smileys", "~/.kde4/share/emoticons", "~/.kde/share/emoticons",
                      EMOTICONS_SUB_PATH};

    // qTox exclusive emoticons
    QStandardPaths::StandardLocation location;
    location = QStandardPaths::AppDataLocation;

    QStringList locations = QStandardPaths::standardLocations(location);
    // system wide emoticons
    locations.append(QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation));
    for (QString qtoxPath : locations) {
        qtoxPath.append(EMOTICONS_SUB_PATH);
        if (!paths.contains(qtoxPath)) {
            paths.append(qtoxPath);
        }
    }

    return paths;
}

/**
 * @brief Wraps passed string into smiley HTML image reference
 * @param key Describes which smiley is needed
 * @return Key that wrapped into image ref
 */
QString getAsRichText(const QString& key)
{
    return RICH_TEXT_PATTERN.arg(key);
}

CSmileyPack::CSmileyPack()
    : cleanupTimer{new QTimer(this)}
{
    loadingMutex.lock();
    QtConcurrent::run(this, &CSmileyPack::load, CGlobal::Instance()->GetFileSmileyPack());
    connect(CGlobal::Instance(), &CGlobal::sigSmileyPackChanged, this,
            &CSmileyPack::onSmileyPackChanged);
    connect(cleanupTimer, &QTimer::timeout, this, &CSmileyPack::cleanupIconsCache);
    cleanupTimer->start(CLEANUP_TIMEOUT);
}

CSmileyPack::~CSmileyPack()
{
    delete cleanupTimer;
}

void CSmileyPack::cleanupIconsCache()
{
    QMutexLocker locker(&loadingMutex);
    for (auto it = cachedIcon.begin(); it != cachedIcon.end();) {
        std::shared_ptr<QIcon>& icon = it->second;
        if (icon.use_count() == 1) {
            it = cachedIcon.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * @brief Returns the singleton instance.
 */
CSmileyPack& CSmileyPack::getInstance()
{
    static CSmileyPack smileyPack;
    return smileyPack;
}

/**
 * @brief Does the same as listSmileyPaths, but with default paths
 */
QList<QPair<QString, QString>> CSmileyPack::listSmileyPacks()
{
    return listSmileyPacks(DEFAULT_PATHS);
}

/**
 * @brief Searches all files called "emoticons.xml" within the every passed path in the depth of 2
 * @param paths Paths where to search for file
 * @return Vector of pairs: {directoryName, absolutePathToFile}
 */
QList<QPair<QString, QString>> CSmileyPack::listSmileyPacks(const QStringList& paths)
{
    QList<QPair<QString, QString>> smileyPacks;
    const QString homePath = QDir::homePath();
    for (QString path : paths) {
        if (path.startsWith('~')) {
            path.replace(0, 1, homePath);
        }

        QDir dir(path);
        if (!dir.exists()) {
            continue;
        }

        for (const QString& subdirectory : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            dir.cd(subdirectory);
            if (dir.exists(EMOTICONS_FILE_NAME)) {
                QString absPath = dir.absolutePath() + QDir::separator() + EMOTICONS_FILE_NAME;
                QPair<QString, QString> p{dir.dirName(), absPath};
                if (!smileyPacks.contains(p)) {
                    smileyPacks.append(p);
                }
            }

            dir.cdUp();
        }
    }

    return smileyPacks;
}

/**
 * @brief Load smile pack
 * @note The caller must lock loadingMutex and should run it in a thread
 * @param filename Filename of smilepack.
 * @return False if cannot open file, true otherwise.
 */
bool CSmileyPack::load(const QString& filename)
{
    QFile xmlFile(filename);
    if (!xmlFile.exists() || !xmlFile.open(QIODevice::ReadOnly)) {
        loadingMutex.unlock();
        return false;
    }

    QDomDocument doc;
    doc.setContent(xmlFile.readAll());
    xmlFile.close();

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
    QDomNodeList emoticonElements = doc.elementsByTagName("emoticon");
    const QString itemName = QStringLiteral("file");
    const QString childName = QStringLiteral("string");
    const int iconsCount = emoticonElements.size();
    emoticons.clear();
    emoticonToPath.clear();
    cachedIcon.clear();

    for (int i = 0; i < iconsCount; ++i) {
        QDomNode node = emoticonElements.at(i);
        QString iconName = node.attributes().namedItem(itemName).nodeValue();
        QString iconPath = QDir{path}.filePath(iconName);
        QDomElement stringElement = node.firstChildElement(childName);
        QStringList emoticonList;
        while (!stringElement.isNull()) {
            QString emoticon = stringElement.text().replace("<", "&lt;").replace(">", "&gt;");
            emoticonToPath.insert(emoticon, iconPath);
            emoticonList.append(emoticon);
            stringElement = stringElement.nextSibling().toElement();
        }

        emoticons.append(emoticonList);
    }

    constructRegex();

    loadingMutex.unlock();
    return true;
}

/**
 * @brief Creates the regex for replacing emoticons with the path to their pictures
 */
void CSmileyPack::constructRegex()
{
    QString allPattern = QStringLiteral("(");

    // construct one big regex that matches on every emoticon
    for (const QString& emote : emoticonToPath.keys()) {
        if (emote.toUcs4().length() == 1) {
            // UTF-8 emoji
            allPattern = allPattern % emote;
        } else {
            // patterns like ":)" or ":smile:", don't match inside a word or else will hit punctuation and html tags
            allPattern = allPattern % QStringLiteral(R"((?<=^|\s))") % QRegularExpression::escape(emote) % QStringLiteral(R"((?=$|\s))");
        }
        allPattern = allPattern % QStringLiteral("|");
    }

    allPattern[allPattern.size() - 1] = QChar(')');

    // compile and optimize regex
    smilify.setPattern(allPattern);
    smilify.optimize();
}

/**
 * @brief Replaces all found text emoticons to HTML reference with its according icon filename
 * @param msg Message where to search for emoticons
 * @return Formatted copy of message
 */
QString CSmileyPack::smileyfied(const QString& msg)
{
    QMutexLocker locker(&loadingMutex);
    QString result(msg);

    int replaceDiff = 0;
    QRegularExpressionMatchIterator iter = smilify.globalMatch(result);
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        int startPos = match.capturedStart();
        int keyLength = match.capturedLength();
        QString imgRichText = getAsRichText(match.captured());
        result.replace(startPos + replaceDiff, keyLength, imgRichText);
        replaceDiff += imgRichText.length() - keyLength;
    }
    return result;
}

/**
 * @brief Returns all emoticons that was extracted from files, grouped by according icon file
 */
QList<QStringList> CSmileyPack::getEmoticons() const
{
    QMutexLocker locker(&loadingMutex);
    return emoticons;
}

/**
 * @brief Gets icon accoring to passed emoticon
 * @param emoticon Passed emoticon
 * @return Returns cached icon according to passed emoticon, null if no icon mapped to this emoticon
 */
std::shared_ptr<QIcon> CSmileyPack::getAsIcon(const QString& emoticon) const
{
    QMutexLocker locker(&loadingMutex);
    if (cachedIcon.find(emoticon) != cachedIcon.end()) {
        return cachedIcon[emoticon];
    }

    const auto iconPathIt = emoticonToPath.find(emoticon);
    if (iconPathIt == emoticonToPath.end()) {
        return std::make_shared<QIcon>();
    }

    const QString& iconPath = iconPathIt.value();
    auto icon = std::make_shared<QIcon>(iconPath);
    cachedIcon[emoticon] = icon;
    return icon;
}

void CSmileyPack::onSmileyPackChanged()
{
    loadingMutex.lock();
    QtConcurrent::run(this, &CSmileyPack::load, CGlobal::Instance()->GetFileSmileyPack());
}
