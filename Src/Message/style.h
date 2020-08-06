
#pragma once

#include <QColor>
#include <QFont>

class QString;
class QWidget;

class Style
{
public:
    enum ColorPalette
    {
        TransferGood,
        TransferWait,
        TransferBad,
        TransferMiddle,
        MainText,
        NameActive,
        StatusActive,
        GroundExtra,
        GroundBase,
        Orange,
        ThemeDark,
        ThemeMediumDark,
        ThemeMedium,
        ThemeLight,
        Action,
        Link,
        SearchHighlighted,
        SelectText
    };

    enum Font
    {
        ExtraBig,
        Big,
        BigBold,
        Medium,
        MediumBold,
        Small,
        SmallLight
    };

    enum MainTheme
    {
        Light,
        Dark
    };

    struct ThemeNameColor {
        MainTheme type;
        QString name;
        QColor color;
    };

    static QStringList getThemeColorNames();
    static const QString getStylesheet(const QString& filename, const QFont& baseFont = QFont());
    static const QString getImagePath(const QString& filename);
    static QString getThemeFolder();
    static QString getThemeName();
    static QColor getColor(ColorPalette entry);
    static QFont getFont(Font font);
    static const QString resolve(const QString& filename, const QFont& baseFont = QFont());
    static void repolish(QWidget* w);
    static void setThemeColor(int color);
    static void setThemeColor(const QColor& color);
    static void applyTheme();
    static QPixmap scaleSvgImage(const QString& path, uint32_t width, uint32_t height);
    static void initPalette();
    static void initDictColor();
    static QString getThemePath();

signals:
    void themeChanged();

private:
    Style();
};
