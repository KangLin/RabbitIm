
#pragma once

#include <QMenu>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QVector>

#include <memory>

class QIcon;

class CEmoticonsWidget : public QMenu
{
    Q_OBJECT
public:
    explicit CEmoticonsWidget(QWidget* parent = nullptr);

signals:
    void insertEmoticon(QString str);

private slots:
    void onEmojiClicked();
    void onPageButtonClicked();
    void PageButtonsUpdate();

protected:
    void mouseReleaseEvent(QMouseEvent* ev) final;
    void mousePressEvent(QMouseEvent* ev) final;
    void wheelEvent(QWheelEvent* event) final;
    void keyPressEvent(QKeyEvent* e) final;

private:
    QStackedWidget stack;
    QVBoxLayout layout;
    QList<std::shared_ptr<QIcon>> emoticonsIcons;

public:
    QSize sizeHint() const override;
};
