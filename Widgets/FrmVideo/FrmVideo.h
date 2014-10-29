/*
 * 视频显示窗体
 * 作者：康林
 * 
 * 注意：本对象是一个单例对象
 */

#ifndef FRMVIDEO_H
#define FRMVIDEO_H

#include <QFrame>
#include "FrmPlayer.h"

namespace Ui {
class CFrmVideo;
}

class CFrmVideo : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmVideo(QWidget *parent = 0);
    virtual ~CFrmVideo();

public slots:
    //显示本地视频  
    void slotDisplayLoacleVideo(const QVideoFrame& frame);
    //显示远端视频  
    void slotDisplayRemoteVideo(const QVideoFrame& frame);
    
protected slots:
private:
    virtual void closeEvent(QCloseEvent *e);
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *event);
    virtual void changeEvent(QEvent*);
    virtual void mouseMoveEvent(QMouseEvent * event);

    //调整显示窗体大小  
    void AdjustPlayer(const QRect &rect);
    //显示/关闭呼叫提示与按钮  
    int ShowWdgInfo(bool bShow);
    //显示/关闭工具栏  
    int ShowToolBar(bool bShow);

private:
    Ui::CFrmVideo *ui;

    CFrmPlayer m_RemotePlayer;//远程视频播放窗口  
    CFrmPlayer m_LocalePlayer;//本地视频播放窗口  

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    void on_cmbCamera_currentIndexChanged(int index);
};

#endif // FRMVIDEO_H
