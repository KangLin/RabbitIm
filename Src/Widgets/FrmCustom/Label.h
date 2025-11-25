#ifndef LABEL_H
#define LABEL_H

#pragma once

#include <QLabel>

class CLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CLabel(QWidget *parent=0, Qt::WindowFlags f = Qt::WindowFlags());
    explicit CLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f = Qt::WindowFlags());

signals:
    // 鼠标单击信号  
    void clicked();

protected:
    // 鼠标单击事件  
    virtual void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // LABEL_H
