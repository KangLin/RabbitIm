#include "WdgScreenEditor.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QApplication>
#include <QScreen>

CWdgScreenEditor::CWdgScreenEditor(QWidget *parent)
    :QWidget(parent),
      toolBar(parent),
      m_ActionGroupComplete(this)
{
    m_penColor = QColor(Qt::red);
    initToolBar();
    toolBar.hide();
}

CWdgScreenEditor::~CWdgScreenEditor()
{
    m_stackImages.clear();
    LOG_MODEL_DEBUG("CWdgScreenEditor", "CWdgScreenEditor::~CWdgScreenEditor");
}

void CWdgScreenEditor::initToolBar(){
    QPixmap pix(50,50);
    QPainter painter(&pix);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    QIcon icon;

    icon = QIcon(":/icon/png/pen.png");
    penBtn.setIcon(icon);
    penBtn.setCursor(Qt::ArrowCursor);
    toolBar.addWidget(&penBtn);
    penBtn.setCheckable(true);
    penBtn.setToolTip(tr("pen"));
    //group->addButton(penBtn);
    connect(&penBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    QPen pen = painter.pen();
    pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(5,5,pix.width() - 10,pix.height() - 10);
    icon = QIcon(pix);
    circleBtn.setIcon(icon);
    circleBtn.setCheckable(true);
    circleBtn.setCursor(Qt::ArrowCursor);
    circleBtn.setToolTip(tr("ellipse tool"));
    toolBar.addWidget(&circleBtn);
    connect(&circleBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    painter.fillRect(pix.rect(),QColor(255,255,255,150));
    painter.drawRect(8,8,pix.width() - 16,pix.height() - 16);
    icon = QIcon(pix);
    rectBtn.setIcon(icon);
    rectBtn.setCheckable(true);
    rectBtn.setCursor(Qt::ArrowCursor);
    rectBtn.setText(tr("rect tool"));
    toolBar.addWidget(&rectBtn);
    connect(&rectBtn,SIGNAL(clicked()),this,SLOT(curOptionChanged()));

    icon = QIcon(":/icon/png/undo.png");
    undoBtn.setIcon(icon);
    undoBtn.setCursor(Qt::ArrowCursor);
    undoBtn.setToolTip(tr("undo"));
    toolBar.addWidget(&undoBtn);
    connect(&undoBtn,SIGNAL(clicked()),this,SLOT(undo()));

    icon = QIcon(":/icon/Color");
    colorBtn.setIcon(icon);
    colorBtn.setToolTip(tr("color"));
    colorBtn.setCursor(Qt::ArrowCursor);
    toolBar.addWidget(&colorBtn);
    connect(&colorBtn,SIGNAL(clicked()),this,SLOT(onSelectColor()));
/*
    icon = QIcon(":/icon/SaveAs");
    saveAsBtn.setIcon(icon);
    saveAsBtn.setToolTip(tr("save as"));
    saveAsBtn.setCursor(Qt::ArrowCursor);
    toolBar.addWidget(&saveAsBtn);
    connect(&saveAsBtn,SIGNAL(clicked()),this,SLOT(saveAs()));
*/
    icon = QIcon(":/icon/Cancel");
    cancelBtn.setIcon(icon);
    cancelBtn.setCursor(Qt::ArrowCursor);
    toolBar.addWidget(&cancelBtn);
    cancelBtn.setToolTip(tr("cancel"));
    connect(&cancelBtn,SIGNAL(clicked()),this,SLOT(onCancelBtnClicked()));

    QAction* pAction = m_menuCompleteBtu.addAction(tr("Clipboard"));
    m_ActionGroupComplete.addAction(pAction);
    m_ActionsComplete.insert(CGlobal::E_TO_CLIPBOARD, pAction);
    pAction = m_menuCompleteBtu.addAction(tr("Save As"));
    m_ActionGroupComplete.addAction(pAction);
    m_ActionsComplete.insert(CGlobal::E_TO_SAVE, pAction);
    pAction = m_menuCompleteBtu.addAction(tr("Send file"));
    m_ActionGroupComplete.addAction(pAction);
    m_ActionsComplete.insert(CGlobal::E_TO_SEND, pAction);
    bool check = connect(&m_ActionGroupComplete, SIGNAL(triggered(QAction*)),
                         SLOT(slotActionGroupCompleteTriggered(QAction*)));
    Q_ASSERT(check);
    connect(&m_menuCompleteBtu, SIGNAL(aboutToShow()), this, SLOT(slotMenuShow()));
    completeBtn.setMenu(&m_menuCompleteBtu);
    m_IconComplete.insert(CGlobal::E_TO_CLIPBOARD, QIcon(":/icon/Cut"));
    m_IconComplete.insert(CGlobal::E_TO_SAVE, QIcon(":/icon/SaveAs"));
    m_IconComplete.insert(CGlobal::E_TO_SEND, QIcon(":/icon/SendFile"));
    completeBtn.setIcon(m_IconComplete[CGlobal::Instance()->GetScreenShotToType()]);
    completeBtn.setToolTip(tr("ok"));
    completeBtn.setCursor(Qt::ArrowCursor);
    completeBtn.setPopupMode(QToolButton::MenuButtonPopup);
    toolBar.addWidget(&completeBtn);
    connect(&completeBtn,SIGNAL(clicked()),this,SLOT(onCompleteBtnClicked()));
}

void CWdgScreenEditor::mouseDoubleClickEvent(QMouseEvent *)
{
    onCompleteBtnClicked();
}

void CWdgScreenEditor::showEvent(QShowEvent *)
{
}

void CWdgScreenEditor::hideEvent(QHideEvent *)
{
    toolBar.hide();
}

void CWdgScreenEditor::resetByImg(const QPixmap &img)
{
    updateForImg(img);
    repaint();
}

QPixmap CWdgScreenEditor::getSelectedImg()
{
    QPixmap pix = QPixmap();
    pix = this->grab(m_image.rect());
    return pix;
}

void CWdgScreenEditor::updateForImg(const QPixmap &img)
{
    m_image = img;

    resize(img.width(), img.height() + 40);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setMouseTracking(true);
    m_tempImg = QImage(img.size(), QImage::Format_ARGB32_Premultiplied);
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
    pen.setColor(m_penColor);
    painter.setPen(pen);
    switch(m_curOption){
    case NoOption:
        break;
    case DrawFree:
        painter.drawLine(m_oldPos, m_curPos);
        break;
    case DrawRect:
        painter.drawRect(m_x, m_y, m_w, m_h);
        break;
    case DrawCircle:
       painter.drawEllipse(m_x, m_y, m_w, m_h);
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
    if(sender() == &penBtn){
        m_curOption = DrawFree;
        QPixmap pix(":/icon/pen.png");
        QCursor cursor(pix,0,pix.height());
        setCursor(cursor);
    }else if(sender() == &circleBtn){
        m_curOption = DrawCircle;
        setCursor(Qt::CrossCursor);
    }else if(sender() == &rectBtn){
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
    pix = this->grab(m_image.rect());
    return pix.save(path);
}

void CWdgScreenEditor::saveAs()
{
    QString szFile;
    QString szFilter =  tr("Images (*.png *.xpm *.jpg *.bmp, *.PPM, *.TIFF, *.XBM)");
    QString szDir = CGlobal::Instance()->GetDirReceiveFile()
            + QDir::separator() + "grabbedImage_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
    szFile = CTool::FileDialog(this, szDir, szFilter, tr("Save"));
    if(!szFile.isEmpty()){
        QFile f(szFile);
        if(f.exists())
        {
            if(QMessageBox::No ==
                    QMessageBox::warning(this, tr("Save"), 
                                         tr("File is exists. Do you save it?"),
                                         QMessageBox::Ok, 
                                         QMessageBox::No))
            {
                onCompleteBtnClicked();
                return;
            }
        }
        if(!save(szFile)){
            QMessageBox::warning(this, tr("Failed to save"), tr("Sorry! Save failed! Please check save path!"));
        }else{
            onCompleteBtnClicked();
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

void CWdgScreenEditor::onSelectColor()
{
     QColor color = QColorDialog::getColor(m_penColor, this, tr("select color"));
     if(color.isValid())
     {
         m_penColor = color;
     }
}

void CWdgScreenEditor::slotActionGroupCompleteTriggered(QAction *act)
{
    QMap<CGlobal::E_SCREEN_SHOT_TO_TYPE, QAction*>::iterator it;
    for(it = m_ActionsComplete.begin(); it != m_ActionsComplete.end(); it++)
    {
        if(it.value() == act)
        {
            CGlobal::Instance()->SetScreenShotToType(it.key());
            completeBtn.setIcon(m_IconComplete[it.key()]);
            break;
        }
    }
}

void CWdgScreenEditor::slotMenuShow()
{
    QMap<CGlobal::E_SCREEN_SHOT_TO_TYPE, QAction*>::iterator it;
    for(it = m_ActionsComplete.begin(); it != m_ActionsComplete.end(); it++)
    {
        it.value()->setChecked(false);
        it.value()->setCheckable(true);
    }
    m_ActionsComplete[CGlobal::Instance()->GetScreenShotToType()]->setChecked(true);
}
