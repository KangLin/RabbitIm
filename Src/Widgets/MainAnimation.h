#ifndef CMAINANIMATION_H
#define CMAINANIMATION_H

#include <QWidget>
#include <QPropertyAnimation>

class CMainAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit CMainAnimation(QWidget *parent = 0);
    virtual void mouseMoveEvent(QMouseEvent *) override;
signals:
    void sigShow();

public slots:    
private:

    // QWidget interface
protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual void enterEvent(QEnterEvent *event) override;
#else
    //事件监听器
    virtual void enterEvent(QEvent* event) override;
#endif
};

#endif // CMAINANIMATION_H
