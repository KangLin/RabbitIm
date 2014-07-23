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

#include "DlgScreenShot.h"

CDlgScreenShot::CDlgScreenShot(QWidget *parent)
    :QDialog(parent,Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint)
{
    resize(qApp->desktop()->size());
    m_bgImg = QImage(size(),QImage::Format_ARGB32_Premultiplied);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setCursor(Qt::CrossCursor);
    drawWindow();
    initSelectParam();
    //===================================
    WId id = qApp->desktop()->winId();
    QRect rect = QRect(m_x,m_y,m_width,m_height).normalized();
    qDebug()<<rect.width();
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWindow(id,rect.x(),rect.y(),rect.width(),rect.height());
    m_editor = new CWdgScreenEditor(pix,this);
    m_editor->hide();
    connect(m_editor,SIGNAL(sigReset()),this,SLOT(onSigReset()));
    connect(m_editor,SIGNAL(sigSelectImg(QPixmap)),this,SLOT(onSigSelectedImg(QPixmap)));
    connect(m_editor,SIGNAL(sigCancel()),this,SLOT(onSigCancel()));
}

QPixmap CDlgScreenShot::getSelectedImg()
{
    return m_selectImg;
}

void CDlgScreenShot::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawImage(rect(),m_bgImg);
}

void CDlgScreenShot::drawWindow()
{
    QPainter painter(&m_bgImg);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(m_bgImg.rect(),QColor(0,0,0,70));
    int penWidth = 2;
    QPen pen = painter.pen();
    pen.setWidth(penWidth);
    painter.setPen(pen);
    painter.fillRect(m_x,m_y,m_width,m_height,Qt::transparent);
    painter.drawRect(m_x - penWidth,m_y - penWidth,m_width + 2 * penWidth,m_height + 2 * penWidth);
    repaint();//即时强制重绘 
}

void CDlgScreenShot::mouseMoveEvent(QMouseEvent *e)
{
    if(!m_bGrabing){
        QWidget::mouseMoveEvent(e);
        return;
    }
    if(e->buttons() & Qt::LeftButton)
    {
        QPoint pos = e->pos();
        m_width = pos.x() - m_x;
        m_height = pos.y() - m_y;
        drawWindow();
    }
}

void CDlgScreenShot::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        qDebug()<<e->pos();
        if(!m_bGrabing){
            QWidget::mousePressEvent(e);
            return;
        }
        m_x = e->pos().x();
        m_y = e->pos().y();
    }
    else if(e->button() == Qt::RightButton)
    {
        if(m_editor->isHidden())
        {
            this->reject();
        }
        else
        {
            m_editor->hide();//改变右键方式  
            onSigReset();
        }
    }
}


void CDlgScreenShot::mouseReleaseEvent(QMouseEvent *e)
{
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
        qDebug()<<rect.width();
        QPixmap pix = QPixmap();
        pix = QPixmap::grabWindow(id,rect.x(),rect.y(),rect.width(),rect.height());
//        ImageEditor* editor = new ImageEditor(QPixmap(),this);
        m_editor->resetByImg(pix);
        m_editor->move(rect.topLeft());//移动到当前选择的rect的左上角
        m_editor->show();
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
    drawWindow();
}

void CDlgScreenShot::onSigSelectedImg(const QPixmap &pix)
{
    QClipboard* clb = QApplication::clipboard();
    clb->setPixmap(pix);
//    clb->setImage(pix.toImage());
    m_selectImg = pix;
    this->accept();
}

void CDlgScreenShot::onSigCancel()
{
//    initSelectParam();
//    drawWindow();
    this->reject();
}

//====================================================================
CWdgScreenEditor::CWdgScreenEditor(const QPixmap& img, QWidget *parent)
    :QWidget(parent),m_image(img)
{
    updateForImg(img);
    initToolBar();
    updateToolBar();
}

void CWdgScreenEditor::initToolBar(){
    toolBar = new QToolBar(this);
    QPixmap pix(50,50);
    QPainter painter(&pix);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    QIcon icon;

    QButtonGroup* group = new QButtonGroup;
    penBtn = new QToolButton;
    icon = QIcon(":/icon/png/pen.png");
    penBtn->setIcon(icon);
    penBtn->setCursor(Qt::ArrowCursor);
    toolBar->addWidget(penBtn);
    penBtn->setCheckable(true);
    penBtn->setToolTip(tr("pen"));
    group->addButton(penBtn);
    connect(penBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    circleBtn = new QToolButton;
    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    QPen pen = painter.pen();
    pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(5,5,pix.width() - 10,pix.height() - 10);
    icon = QIcon(pix);
    circleBtn->setIcon(icon);
    circleBtn->setCheckable(true);
    circleBtn->setCursor(Qt::ArrowCursor);
    circleBtn->setToolTip(tr("ellipse tool"));
    group->addButton(circleBtn);
    toolBar->addWidget(circleBtn);
    connect(circleBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    rectBtn = new QToolButton;
    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    painter.drawRect(8,8,pix.width() - 16,pix.height() - 16);
    icon = QIcon(pix);
    rectBtn->setIcon(icon);
    rectBtn->setCheckable(true);
    rectBtn->setCursor(Qt::ArrowCursor);
    rectBtn->setText(tr("rect tool"));
    group->addButton(rectBtn);
    toolBar->addWidget(rectBtn);
    connect(rectBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    undoBtn = new QToolButton;
    icon = QIcon(":/icon/png/undo.png");
    undoBtn->setIcon(icon);
    undoBtn->setCursor(Qt::ArrowCursor);
    undoBtn->setToolTip(tr("undo"));
    toolBar->addWidget(undoBtn);
    connect(undoBtn,SIGNAL(clicked()),this,SLOT(undo()));

    saveAsBtn = new QToolButton;
    icon = QIcon(":/icon/png/saveas.png");
    saveAsBtn->setIcon(icon);
    saveAsBtn->setToolTip(tr("save as"));
    saveAsBtn->setCursor(Qt::ArrowCursor);
    toolBar->addWidget(saveAsBtn);
    connect(saveAsBtn,SIGNAL(clicked()),this,SLOT(saveAs()));

    cancelBtn = new QToolButton;
    icon = QIcon(":/icon/png/cancel.png");
    cancelBtn->setIcon(icon);
    cancelBtn->setCursor(Qt::ArrowCursor);
    toolBar->addWidget(cancelBtn);
    cancelBtn->setToolTip(tr("cancel"));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(onCancelBtnClicked()));

    completeBtn = new QToolButton;
    icon = QIcon(":/icon/png/complete.png");
    completeBtn->setIcon(icon);
    completeBtn->setToolTip(tr("ok"));
    completeBtn->setCursor(Qt::ArrowCursor);
    toolBar->addWidget(completeBtn);
    connect(completeBtn,SIGNAL(clicked()),this,SLOT(onCompleteBtnClicked()));
}

//void ZScreenEditor::moveEvent(QMoveEvent* e)
//{
//    if(frameGeometry().bottom() + toolBar->height() > qApp->desktop()->height()){
//        resize(m_image.size());
//        toolBar->move(0,m_image.height() - toolBar->height());
//    }
//}

void CWdgScreenEditor::mouseDoubleClickEvent(QMouseEvent *)
{
    onCompleteBtnClicked();
}

void CWdgScreenEditor::resetByImg(const QPixmap &img)
{
    updateForImg(img);
    updateToolBar();
    repaint();
}

QPixmap CWdgScreenEditor::getSelectedImg()
{
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWidget(this,m_image.rect());
    return pix;
}

void CWdgScreenEditor::updateToolBar()
{
    toolBar->move(0,m_image.height());
    setMinimumSize(250,m_image.height() + toolBar->height());
}

void CWdgScreenEditor::updateForImg(const QPixmap &img)
{
    m_image = img;

    resize(img.width(),img.height() + 40);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setMouseTracking(true);
    m_tempImg = QImage(img.size(),QImage::Format_ARGB32_Premultiplied);
    m_tempImg.fill(Qt::transparent);
    m_bufferImg = m_tempImg;
    m_stackImages.clear();
    m_stackImages.push_back(m_tempImg);
     m_x = 0;
     m_y = 0;
     m_w = 0;
     m_h = 0;
}

void CWdgScreenEditor::paint()
{
    QPainter painter(&m_tempImg);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    switch(m_curOption){
    case NoOption:
        break;
    case DrawFree:
        painter.drawLine(m_oldPos,m_curPos);
        break;
    case DrawRect:
        painter.drawRect(m_x,m_y,m_w,m_h);
        break;
    case DrawCircle:
       painter.drawEllipse(m_x,m_y,m_w,m_h);
       break;
    }
    update();
}

void CWdgScreenEditor::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::LeftButton){
        if(!m_image.rect().contains(e->pos())){
            QWidget::mousePressEvent(e);
            return;
        }
        m_bDrawing = true;
        m_x = e->pos().x();
        m_y = e->pos().y();
        m_curPos = e->pos();
        switch(m_curOption){
        case NoOption:break;
        case DrawRect:
        case DrawCircle:
            setCursor(Qt::CrossCursor);
            break;
        case DrawFree:
            QPixmap pix(":/icon/png/pen.png");
            QCursor cursor(pix,0,pix.height());
            setCursor(cursor);
            break;
        }
    }else if(e->button() == Qt::RightButton){
        emit sigReset();
        this->close();
    }

}

void CWdgScreenEditor::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        if(!m_image.rect().contains(e->pos())){
            QWidget::mouseMoveEvent(e);
            return;
        }
        switch(m_curOption){
        case DrawFree:
            m_oldPos = m_curPos;
            m_curPos = e->pos();
            break;
        case DrawRect:
        case DrawCircle:
            m_tempImg = m_bufferImg;
            m_w = e->pos().x() - m_x;
            m_h = e->pos().y() - m_y;
            break;
        case NoOption:
        default:
            break;
        }
        paint();
    }
}

void CWdgScreenEditor::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_bDrawing = false;
        m_bufferImg = m_tempImg;
        m_w = 0;
        m_h = 0;
        m_stackImages.push_back(m_tempImg);
    }
}
void CWdgScreenEditor::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(0,0,m_image);
    if(m_bDrawing){
        painter.drawImage(0,0,m_tempImg);
    }else{
        painter.drawImage(0,0,m_bufferImg);
    }
}

void CWdgScreenEditor::curOptionChanged()
{
    if(sender() == penBtn){
        m_curOption = DrawFree;
        QPixmap pix(":/icon/pen.png");
        QCursor cursor(pix,0,pix.height());
        setCursor(cursor);
    }else if(sender() == circleBtn){
        m_curOption = DrawCircle;
        setCursor(Qt::CrossCursor);
    }else if(sender() == rectBtn){
        m_curOption = DrawRect;
        setCursor(Qt::CrossCursor);
    }
}

void CWdgScreenEditor::undo()
{
    if(m_stackImages.size() <= 1) return;

    m_stackImages.pop_back();
    m_tempImg = m_stackImages.last();
    m_bufferImg = m_stackImages.last();
    update();
}

bool CWdgScreenEditor::save(const QString& path)
{
    QPixmap pix = QPixmap();
    pix = QPixmap::grabWidget(this,m_image.rect());
    return pix.save(path);
}

void CWdgScreenEditor::saveAs()
{
    QString filter = "*.png;;*.jpg;;*.bmp;;*.gif;;*.jpeg";
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +QDir::separator() +  "grabbedImage.png";
//    QString dir = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) +QDir::separator() +  "grabbedImage.png";
    QString path = QFileDialog::getSaveFileName(0,tr("Save"),dir,filter);
    if(!path.isNull()){
        if(!save(path)){
            QMessageBox::warning(this,tr("failed to save"),tr("Sorry! Save failed!Please check savePath!"));
        }else{
//            this->rej
        }
    }
}

void CWdgScreenEditor::onCancelBtnClicked()
{
    this->close();
    emit sigCancel();
}

void CWdgScreenEditor::onCompleteBtnClicked()
{
    QPixmap pix = getSelectedImg();
    emit sigSelectImg(pix);
    this->close();
}
