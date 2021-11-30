#ifndef WDGSCREENEDITOR_H
#define WDGSCREENEDITOR_H

#include <QWidget>

#include <QStack>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QActionGroup>
#include <QMouseEvent>
#include <QHideEvent>
#include <QShowEvent>
#include "Global/Global.h"

class CWdgScreenEditor : public QWidget
{
    Q_OBJECT
public:
    CWdgScreenEditor(QWidget* parent = 0);
    ~CWdgScreenEditor();
    void resetByImg(const QImage& img);
    QPixmap getSelectedImg();

private:
    enum Option{NoOption,DrawFree,DrawCircle,DrawRect};

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent* );

private:
    void updateForImg(const QImage& img);
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

    //完成按钮的功能菜单响应曹函数  
    void slotMenuShow();//菜单显示时触发  
    void slotActionGroupCompleteTriggered(QAction* act);

signals:
    void sigReset();
    void sigSelectImg(const QPixmap& pix);
    void sigCancel();

public:
    QToolBar toolBar;
private:
    QToolButton circleBtn;
    QToolButton rectBtn;
    QToolButton undoBtn;
    QToolButton textBtn;
    QToolButton penBtn;
    QToolButton colorBtn;
    QToolButton saveAsBtn;
    QToolButton cancelBtn;
    QToolButton completeBtn;
    
    QMap<CGlobal::E_SCREEN_SHOT_TO_TYPE, QIcon> m_IconComplete;
    QMenu m_menuCompleteBtu;
    QMap<CGlobal::E_SCREEN_SHOT_TO_TYPE, QAction*> m_ActionsComplete;
    QActionGroup m_ActionGroupComplete;

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


#endif // WDGSCREENEDITOR_H
