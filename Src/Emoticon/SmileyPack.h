#pragma once

#include <QIcon>
#include <QMap>
#include <QMutex>
#include <QRegularExpression>

#include <memory>

class QTimer;

class CSmileyPack : public QObject
{
    Q_OBJECT

public:
    static CSmileyPack& getInstance();
    static QList<QPair<QString, QString>> listSmileyPacks(const QStringList& paths);
    static QList<QPair<QString, QString>> listSmileyPacks();

    QString smileyfied(const QString& msg);
    QList<QStringList> getEmoticons() const;
    std::shared_ptr<QIcon> getAsIcon(const QString& key) const;

private slots:
    void onSmileyPackChanged();
    void cleanupIconsCache();

private:
    CSmileyPack();
    CSmileyPack(CSmileyPack&) = delete;
    CSmileyPack& operator=(const CSmileyPack&) = delete;
    ~CSmileyPack() override;

    bool load(const QString& filename);
    void constructRegex();

    mutable std::map<QString, std::shared_ptr<QIcon>> cachedIcon;
    QHash<QString, QString> emoticonToPath;
    QList<QStringList> emoticons;
    QString path;
    QTimer* cleanupTimer;
    QRegularExpression smilify;
    mutable QMutex loadingMutex;
};
