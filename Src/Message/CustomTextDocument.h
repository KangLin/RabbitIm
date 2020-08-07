#pragma once

#include <QTextDocument>
#include <QList>

#include <memory>

class QIcon;

class CCustomTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CCustomTextDocument(QObject* parent = nullptr);

protected:
    virtual QVariant loadResource(int type, const QUrl& name);

private:
    QList<std::shared_ptr<QIcon>> emoticonIcons;
};
