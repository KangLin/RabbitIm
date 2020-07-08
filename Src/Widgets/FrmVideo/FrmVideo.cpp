/*
 * 视频显示窗体
 * 作者：康林
 * 
 * 注意：本对象是一个单例对象
 */

#include "Tool.h"
#include "FrmVideo.h"
#include "ui_FrmVideo.h"
#include "../../Global/Global.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

CFrmVideo::CFrmVideo(QWidget *parent) :
    QFrame(parent),
    m_RemotePlayer(this),
    m_LocalePlayer(this),
    ui(new Ui::CFrmVideo)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::CFrmVideo");
    ui->setupUi(this);

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);

    /*设置提示文本颜色  
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    ui->lbPrompt->setPalette(pe);//*/

    ShowWdgInfo(false);
    //当tracking为off时，只有当一个鼠标键按下时，才会有mouseEvent事件。  
    //当tracking为on时，没鼠标键按下，也会有mouseEvent事件  
    this->setMouseTracking(true);

    bool check = connect(GET_MAINWINDOW, SIGNAL(sigRefresh()), 
                         SLOT(slotRefresh()));
    Q_ASSERT(check);
    
    slotRefresh();
}

CFrmVideo::~CFrmVideo()
{
    delete ui;
}

void CFrmVideo::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    LOG_MODEL_DEBUG("Video", "CFrmVideo::closeEvent");
}

//调整视频播放窗口的位置  
void CFrmVideo::AdjustPlayer(const QRect &rect)
{
    m_RemotePlayer.setGeometry(rect);
    QPoint point(rect.topLeft());
    point.setX(point.x() + (rect.width() - (rect.width() >> 2)));
    point.setY(point.y() + (rect.height() - (rect.height() >> 2)));
    QRect localeRect(point, rect.bottomRight());
    m_LocalePlayer.setGeometry(localeRect);
}

void CFrmVideo::resizeEvent(QResizeEvent *)
{
    //得到视频的高宽  
    double nWidth = 320;
    double nHeight = 240;
    //TODO:增加帧的长宽  

    QRect rect = this->rect();
    QRect rectAdjust = this->rect();

    //计算宽度  
    double frameAspectRatio = nWidth / nHeight;
    double frmAspectRatio = (double)rect.width() / (double)rect.height();
    if(frameAspectRatio < frmAspectRatio)
    {
        //以窗体高度为基准计算宽度  
        int nW = rect.height() * frameAspectRatio;
        rectAdjust.setLeft(rect.left() + ((rect.width() - nW) >> 1));
        rectAdjust.setRight(rect.right() - ((rect.width() - nW) >> 1));
    }
    else
    {
        //以宽度为基准计算高度  
        double ratio = nHeight / nWidth;
        int nH = rect.width() * ratio;
        rectAdjust.setTop(rect.top() + ((rect.height() - nH) >> 1));
        rectAdjust.setBottom(rect.bottom() - ((rect.height() - nH) >> 1));
    }

    AdjustPlayer(rectAdjust);
}

void CFrmVideo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    //QLinearGradient gradient(0, rect().height() >> 2, rect().right(), this->rect().height() >> 2);
    /*
    QRadialGradient gradient(rect().width() >> 2, rect().height() >> 2,
                             rect().height() >> 2,
                             rect().width() >> 2, rect().height() >> 2);//*/
    /*
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(0.5, Qt::green);
    gradient.setColorAt(1, Qt::blue);

    QBrush bg(gradient);//*/
    /*
    QPainter painter(this);
    QBrush bg(Qt::darkBlue);
    painter.setBrush(bg);
    painter.drawRect(rect());//*/
}

void CFrmVideo::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

//当tracking为off时，只有当一个鼠标键按下时，才会有mouseEvent事件。  
//当tracking为on时，没鼠标键按下，也会有mouseEvent事件  
void CFrmVideo::mouseMoveEvent(QMouseEvent *event)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::mouseMoveEvent");
    Q_UNUSED(event);
    //ShowToolBar(true);
}

int CFrmVideo::ShowWdgInfo(bool bShow)
{
    if(bShow)
    {
        ui->ToolBar->show();
        ui->wdgInfo->show();
        ui->pbOK->setEnabled(true);
    }
    else
    {
        ui->ToolBar->hide();
        ui->wdgInfo->hide();
        ui->pbOK->setEnabled(false);
    }
    return 0;
}

int CFrmVideo::ShowToolBar(bool bShow)
{
    if(bShow)
    {
        ui->ToolBar->show();
        ui->wdgInfo->show();
    }
    else
    {
        ui->wdgInfo->hide();
        ui->ToolBar->hide();
    }
    return 0;
}

void CFrmVideo::slotDisplayLocaleVideo(const QVideoFrame &frame)
{
    m_LocalePlayer.slotPresent(frame);
}

void CFrmVideo::slotDisplayLocaleVideo(const QImage &frame)
{
    m_LocalePlayer.slotPresent(frame);
}

void CFrmVideo::slotDisplayRemoteVideo(const QVideoFrame &frame)
{
    m_RemotePlayer.slotPresent(frame);
}

void CFrmVideo::slotDisplayRemoteVideo(const QImage &frame)
{
    m_RemotePlayer.slotPresent(frame);
}

void CFrmVideo::on_pbOK_clicked()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_pbOK_clicked");
}

void CFrmVideo::on_pbCancel_clicked()
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_pbCancel_clicked");
}

void CFrmVideo::on_cmbCamera_currentIndexChanged(int index)
{
    LOG_MODEL_DEBUG("Video", "CFrmVideo::on_cmbCamera_currentIndexChanged");
    Q_UNUSED(index);
}

// 根据配置打开或隐藏本地窗口  
void CFrmVideo::slotRefresh()
{
    if(CGlobal::Instance()->GetIsShowLocaleVideo())
        m_LocalePlayer.show();
    else
        m_LocalePlayer.hide();
}
