#ifndef DLGSCREENSHOT_H
#define DLGSCREENSHOT_H

#include <QWidget>
#include <QDialog>
#include "WdgScreenEditor.h"

class QComboBox;
class QPushButton;
class CWdgScreenEditor;

class CDlgScreenShot : public QDialog
{
    Q_OBJECT

public:
    CDlgScreenShot(QWidget* parent = 0);
    virtual ~CDlgScreenShot();
    QPixmap getSelectedImg();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void initSelectParam();
    QImage drawWindow();

private slots:
    void onSigReset();
    void onSigSelectedImg(const QPixmap& pix);
    void onSigCancel();

//signals:
//    void sigSelectImg(const QPixmap& pix);

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_bGrabing;
    QImage m_imgDesktop;
    CWdgScreenEditor m_Editor;
    QPixmap m_selectImg;
};

#endif // DLGSCREENSHOT_H
