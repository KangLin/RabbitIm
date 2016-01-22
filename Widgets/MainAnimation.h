#ifndef CMAINANIMATION_H
#define CMAINANIMATION_H

#include <QWidget>
#include <QPropertyAnimation>

class CMainAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit CMainAnimation(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *);
    //事件监听器  
    void enterEvent(QEvent* event);
signals:
    void sigShow();

public slots:    
private:
};

#endif // CMAINANIMATION_H
