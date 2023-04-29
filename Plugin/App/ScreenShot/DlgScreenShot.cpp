#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QImageWriter>
#include <QPaintEvent>
#include <QDebug>
#include <QDir>
#include <QToolBar>
#include <QPainter>
#include <QToolButton>
#include <QPixmap>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QDesktopServices>
#include <QImageWriter>
#include <QMessageBox>
#include <QClipboard>
#include <QColorDialog>
#include <QScreen>
#include <QImage>
#include "DlgScreenShot.h"
#include "Global/Global.h"

CDlgScreenShot::CDlgScreenShot(QWidget *parent)
    :QDialog(parent,
             Qt::FramelessWindowHint
//             #ifndef WIN32
//             | Qt::X11BypassWindowManagerHint  //这个标志是在x11下有用,查看帮助QWidget::showFullScreen(),符合ICCCM协议的不需要这个
//             #endif
             | Qt::Tool
             | Qt::WindowStaysOnTopHint
             | Qt::CustomizeWindowHint
             ),
    m_x(0),
    m_y(0),
    m_width(0),
    m_height(0),
    m_Editor(this)
{
    QDesktopWidget* pDesktop = qApp->desktop();
    this->setFixedSize(pDesktop->size());
    resize(pDesktop->size());
    setAttribute(Qt::WA_TranslucentBackground, true);
    setCursor(Qt::CrossCursor);

    m_imgDesktop = GetScreenShot(rect().x(), rect().y(), rect().width(), rect().height());
    
    initSelectParam();

    m_Editor.hide();
    connect(&m_Editor, SIGNAL(sigReset()), this, SLOT(onSigReset()));
    connect(&m_Editor, SIGNAL(sigSelectImg(QPixmap)),
            this, SLOT(onSigSelectedImg(QPixmap)));
    connect(&m_Editor, SIGNAL(sigCancel()), this, SLOT(onSigCancel()));
}

CDlgScreenShot::~CDlgScreenShot()
{
    qDebug("CDlgScreenShot::~CDlgScreenShot");
}

QImage CDlgScreenShot::GetScreenShot(int x, int y, int w, int h)
{
    QDesktopWidget* pDesktop = qApp->desktop();
    
    //        QPixmap pix(pDesktop->size());
    //        pDesktop->render(&pix);
    //        QImage img = pix.copy(x, y, w, h).toImage();
    //        return img;
    
    WId id = qApp->desktop()->winId();  
    QScreen *pScreen = QGuiApplication::primaryScreen();
    QPoint pos = mapToGlobal(QPoint(x, y));
    return pScreen->grabWindow(id, pos.x(), pos.y(), w, h).toImage();
    
}

QPixmap CDlgScreenShot::getSelectedImg()
{
    return m_selectImg;
}

void CDlgScreenShot::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.drawImage(rect(), drawWindow());
}

QImage CDlgScreenShot::drawWindow()
{
    QImage img;
    img = m_imgDesktop;
    if(img.isNull())
        return img;
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(img.rect(), QColor(0, 0, 0, 70));
    int penWidth = 2;
    QPen pen = painter.pen();
    pen.setWidth(penWidth);
    painter.setPen(pen);
    painter.fillRect(m_x, m_y, m_width, m_height, Qt::transparent);
    painter.drawRect(m_x - penWidth, m_y - penWidth,
                     m_width + 2 * penWidth,
                     m_height + 2 * penWidth);
    return img;
}

void CDlgScreenShot::mouseMoveEvent(QMouseEvent *e)
{
    qDebug("mouseMoveEvent:e->pos:x:%d;y:%d;QCursor::pos:x:%d;y:%d",
                    e->pos().x(), e->pos().y(),
                    QCursor::pos().x(), QCursor::pos().y());
    if(!m_bGrabing){
        QWidget::mouseMoveEvent(e);
        return;
    }
    if(e->buttons() & Qt::LeftButton)
    {
        QPoint pos = e->pos();
        m_width = pos.x() - m_x;
        m_height = pos.y() - m_y;
        this->update();
    }
}

void CDlgScreenShot::mousePressEvent(QMouseEvent *e)
{
    qDebug("mousePressEvent:e->pos:x:%d;y:%d;QCursor::pos:x:%d;y:%d",
                    e->pos().x(), e->pos().y(),
                    QCursor::pos().x(), QCursor::pos().y());
    if(e->button() == Qt::LeftButton)
    {
        if(!m_bGrabing){
            QWidget::mousePressEvent(e);
            return;
        }
        QPoint pos = e->pos(); //QCursor::pos();
        m_x = pos.x();
        m_y = pos.y();
    }
    else if(e->button() == Qt::RightButton)
    {
        setCursor(Qt::CrossCursor);
        if(m_Editor.isHidden())
        {
            this->reject();
        }
        else
        {
            m_Editor.hide();//改变右键方式  
            onSigReset();
        }
    }
}

void CDlgScreenShot::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug("mouseReleaseEvent:e->pos:x:%d;y:%d;QCursor::pos:x:%d;y:%d",
                    e->pos().x(), e->pos().y(),
                    QCursor::pos().x(), QCursor::pos().y());
    if(!m_bGrabing)
    {
        QWidget::mouseReleaseEvent(e);
        return;
    }
    if(e->button() == Qt::LeftButton)
    {
        m_bGrabing = false;
        setCursor(Qt::ArrowCursor);
        WId id = qApp->desktop()->winId();
        QRect rect = QRect(m_x,m_y,m_width,m_height).normalized();
        qDebug("x:%d;y:%d;width:%d;height:%d;DlgWidth:%d;DlgHeight:%d",
                        rect.x(),
                        rect.y(),
                        rect.width(),
                        rect.height(),
                        this->width(),
                        this->height());
        
        int x = rect.x(), y = rect.y() + rect.height();
        m_Editor.toolBar.show(); //需要先显示，才能得到正确的大小  
        QRect rectToolBar = m_Editor.toolBar.frameGeometry();
        qDebug("x:%d;y:%d;width:%d;height:%d",
                        rectToolBar.x(),
                        rectToolBar.y(),
                        rectToolBar.width(),
                        rectToolBar.height());
        if(y + rectToolBar.height() >= this->height())
        {
            y = rect.y() - rectToolBar.height();
        }
        else if(x + rectToolBar.width() >= this->width())
        {
            x = this->width() - rectToolBar.width();
            qCritical("x:%d;y:%d;width:%d;height:%d;toolx:%d",
                            rectToolBar.x(),
                            rectToolBar.y(),
                            rectToolBar.width(),
                            rectToolBar.height(),
                            x);
        }
        m_Editor.toolBar.move(x, y);
        m_Editor.resetByImg(GetScreenShot(rect.x(), rect.y(), rect.width(), rect.height()));
        m_Editor.move(rect.topLeft());//移动到当前选择的rect的左上角  
        m_Editor.show();
    }
}

void CDlgScreenShot::initSelectParam()
{
    m_bGrabing = true;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
}

void CDlgScreenShot::onSigReset()
{
    initSelectParam();
    this->update();
}

void CDlgScreenShot::onSigSelectedImg(const QPixmap &pix)
{
    m_selectImg = pix;
    this->accept();
}

void CDlgScreenShot::onSigCancel()
{
    this->reject();
}
