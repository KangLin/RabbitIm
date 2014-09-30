#ifndef EMOTICONSWIDGET_H
#define EMOTICONSWIDGET_H

#include <QMenu>
#include <QStackedWidget>
#include <QVBoxLayout>

class CEmoticonsWidget : public QMenu
{
    Q_OBJECT
public:
    explicit CEmoticonsWidget(QWidget *parent = 0);

signals:
    void insertEmoticon(const QString &str);

private slots:
    void onSmileyClicked();
    void onPageButtonClicked();

private:
    QStackedWidget stack;
    QVBoxLayout layout;

public:
    virtual QSize sizeHint() const;
};

#endif // EMOTICONSWIDGET_H
