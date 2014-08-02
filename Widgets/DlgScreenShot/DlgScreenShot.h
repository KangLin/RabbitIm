#ifndef DLGSCREENSHOT_H
#define DLGSCREENSHOT_H

#include <QWidget>
#include <QDialog>
#include <QStack>
class QComboBox;
class QPushButton;
class CWdgScreenEditor;

class CDlgScreenShot : public QDialog
{
    Q_OBJECT

public:
    CDlgScreenShot(QWidget* parent = 0);
    QPixmap getSelectedImg();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void initSelectParam();
    void drawWindow();

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
    QImage m_bgImg;
    CWdgScreenEditor* m_editor;
    QPixmap m_selectImg;
};


//================================================================
class QToolButton;
class QToolBar;
enum Option{NoOption,DrawFree,DrawCircle,DrawRect};

class  CWdgScreenEditor : public QWidget
{
    Q_OBJECT
public:
    CWdgScreenEditor(const QPixmap& img,QWidget* parent = 0);
    void resetByImg(const QPixmap& img);
    QPixmap getSelectedImg();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
//    void moveEvent(QMoveEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    void updateForImg(const QPixmap& img);
    void updateToolBar();
    void initToolBar();
    void paint();
    bool save(const QString& path);

private slots:
    void curOptionChanged();
    void undo();
    void saveAs();
    void onCancelBtnClicked();
    void onCompleteBtnClicked();
    void onSelectColor();

signals:
    void sigReset();
    void sigSelectImg(const QPixmap& pix);
    void sigCancel();

private:
    QToolBar* toolBar;
    QToolButton* saveAsBtn;
    QToolButton* cancelBtn;
    QToolButton* completeBtn;
    QToolButton* circleBtn;
    QToolButton* rectBtn;
    QToolButton* undoBtn;
    QToolButton* textBtn;
    QToolButton* penBtn;
    QToolButton* colorBtn;

    Option m_curOption;
    QPixmap m_image;
    QImage m_tempImg;
    QImage m_bufferImg;
    QStack<QImage> m_stackImages;
    bool m_bDrawing;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    QPoint m_curPos;
    QPoint m_oldPos;
    
    QColor m_penColor;
};

#endif // DLGSCREENSHOT_H
