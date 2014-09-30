
#ifndef STYLE_H
#define STYLE_H

class QString;

class CStyle
{
public:
    static QString get(const QString& filename);
private:
    CStyle();
};

#endif // STYLE_H
