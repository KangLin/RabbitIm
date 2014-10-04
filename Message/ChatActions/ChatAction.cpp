#include "ChatAction.h"
#include <QStringList>
#include <QBuffer>
#include "Global/Global.h"
#include <initializer_list>

QString CChatAction::toHtmlChars(const QString &str)
{
    static QList<QPair<QString, QString>> replaceList = {
        {"\"", "&quot;"}, 
        {"&","&amp;"}, 
        {">","&gt;"}, 
        {"<","&lt;"}
    };
    QString res = str;

    for (auto &it : replaceList)
        res = res.replace(it.first,it.second);

    return res;
}

QString CChatAction::QImage2base64(const QImage &img)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG"); // writes image into ba in PNG format
    return ba.toBase64();
}

QString CChatAction::getName()
{
    QString name;
    if (isMe)
    {
        name = GLOBAL_USER->GetUserInfoLocale()->GetInfo()->GetShowName();
        return QString("<div class=name_me>" + toHtmlChars(name) + "</div>");
    }

    name = GLOBAL_USER->GetUserInfoRoster(szId)->GetInfo()->GetShowName();
    return QString("<div class=name>" + toHtmlChars(name) + "</div>");
}

QString CChatAction::getDate()
{
    QString res = date;
    return res;
}
