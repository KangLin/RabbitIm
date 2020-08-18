/**
  @brief: The form is display QImage
  @author: Kang Lin<kl222@126.com>
  */

#ifndef CFRMDISPLAY_H
#define CFRMDISPLAY_H

#include <QWidget>
#include <QImage>
#include <QVideoFrame>
#include <QMetaClassInfo>
#include "../../Global/Global.h"

class RABBITIM_SHARED_LIBRARY CFrmDisplay : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Kang Lin <kl222@126.com>")
    Q_CLASSINFO("Description", "The form is display QImage")

public:
    explicit CFrmDisplay(QWidget *parent = nullptr);
    virtual ~CFrmDisplay();
    
public Q_SLOTS:
    void slotDisplay(const QImage &image);
    int slotSetAspectRatio(bool bAspectRation = true);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage m_Image;
    bool m_bAspectRation;
};

#endif // CFRMDISPLAY_H
