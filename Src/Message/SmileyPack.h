
#ifndef SMILEYPACK_H
#define SMILEYPACK_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>

//maps emoticons to smileys
class CSmileyPack : public QObject
{
    Q_OBJECT
public:
    static CSmileyPack& getInstance();
    static QList<QPair<QString, QString> > listSmileyPacks(QStringList &paths);
    static bool isValid(const QString& filename);

    bool load(const QString& filename);
    QString smileyfied(QString msg);
    QList<QStringList> getEmoticons() const;
    QString getAsRichText(const QString& key);
    QIcon getAsIcon(const QString& key);

private slots:
    void onSmileyPackChanged();

private:
    CSmileyPack();
    CSmileyPack(CSmileyPack&) = delete;
    CSmileyPack& operator=(const CSmileyPack&) = delete;

    void cacheSmiley(const QString& name);
    QByteArray getCachedSmiley(const QString& key);

    QHash<QString, QString> filenameTable; // matches an emoticon to its corresponding smiley ie. ":)" -> "happy.png"
    QHash<QString, QByteArray> imgCache; // (scaled) representation of a smiley ie. "happy.png" -> data
    QList<QStringList> emoticons; // {{ ":)", ":-)" }, {":(", ...}, ... }
    QString path; // directory containing the cfg and image files
};

#endif // SMILEYPACK_H
