
#include "EmoticonsWidget.h"
#include "SmileyPack.h"
#include "Global/Global.h"

#include <QFile>
#include <QGridLayout>
#include <QLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QRadioButton>

#include <math.h>

CEmoticonsWidget::CEmoticonsWidget(QWidget* parent)
    : QMenu(parent)
{
    //setStyleSheet(Style::getStylesheet("emoticonWidget/emoticonWidget.css"));
    setLayout(&layout);
    layout.addWidget(&stack);

    QWidget* pageButtonsContainer = new QWidget;
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    pageButtonsContainer->setLayout(buttonLayout);

    layout.addWidget(pageButtonsContainer);

    const int maxCols = 8;
    const int maxRows = 8;
    const int itemsPerPage = maxRows * maxCols;

    const QList<QStringList>& emoticons = CSmileyPack::getInstance().getEmoticons();
    int itemCount = emoticons.size();
    int pageCount = ceil(float(itemCount) / float(itemsPerPage));
    int currPage = 0;
    int currItem = 0;
    int row = 0;
    int col = 0;

    // respect configured emoticon size
    const int px = CGlobal::Instance()->GetEmojiFontPointSize();
    const QSize size(px, px);

    // create pages
    buttonLayout->addStretch();
    for (int i = 0; i < pageCount; ++i) {
        QGridLayout* pageLayout = new QGridLayout;
        pageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding),
                            maxRows, 0);
        pageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 0,
                            maxCols);

        QWidget* page = new QWidget;
        page->setLayout(pageLayout);
        stack.addWidget(page);

        // page buttons are only needed if there is more than 1 page
        if (pageCount > 1) {
            QRadioButton* pageButton = new QRadioButton;
            pageButton->setProperty("pageIndex", i);
            pageButton->setCursor(Qt::PointingHandCursor);
            pageButton->setChecked(i == 0);
            buttonLayout->addWidget(pageButton);

            connect(pageButton, &QRadioButton::clicked, this, &CEmoticonsWidget::onPageButtonClicked);
        }
    }
    buttonLayout->addStretch();

    CSmileyPack& smileyPack = CSmileyPack::getInstance();
    for (const QStringList& set : emoticons) {
        QPushButton* button = new QPushButton;
        std::shared_ptr<QIcon> icon = smileyPack.getAsIcon(set[0]);
        emoticonsIcons.append(icon);
        button->setIcon(icon->pixmap(size));
        button->setToolTip(set.join(" "));
        button->setProperty("sequence", set[0]);
        button->setCursor(Qt::PointingHandCursor);
        button->setFlat(true);
        button->setIconSize(size);
        button->setFixedSize(size);

        connect(button, &QPushButton::clicked, this, &CEmoticonsWidget::onEmojiClicked);

        qobject_cast<QGridLayout*>(stack.widget(currPage)->layout())->addWidget(button, row, col);

        ++col;
        ++currItem;

        // next row
        if (col >= maxCols) {
            col = 0;
            ++row;
        }

        // next page
        if (currItem >= itemsPerPage) {
            row = 0;
            currItem = 0;
            ++currPage;
        }
    }

    // calculates sizeHint
    layout.activate();
}

void CEmoticonsWidget::onEmojiClicked()
{
    // emit insert emoticon
    QWidget* sender = qobject_cast<QWidget*>(QObject::sender());
    if (sender) {
        QString sequence =
            sender->property("sequence").toString().replace("&lt;", "<").replace("&gt;", ">");
        this->hide();
        emit insertEmoticon(sequence);
    }
}

void CEmoticonsWidget::onPageButtonClicked()
{
    QWidget* sender = qobject_cast<QRadioButton*>(QObject::sender());
    if (sender) {
        int page = sender->property("pageIndex").toInt();
        stack.setCurrentIndex(page);
    }
}

QSize CEmoticonsWidget::sizeHint() const
{
    return layout.sizeHint();
}

void CEmoticonsWidget::mouseReleaseEvent(QMouseEvent* ev)
{
    if (!rect().contains(ev->pos()))
        hide();
}

void CEmoticonsWidget::mousePressEvent(QMouseEvent*)
{
}

void CEmoticonsWidget::wheelEvent(QWheelEvent* e)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    const bool vertical = qAbs(e->angleDelta().y()) >= qAbs(e->angleDelta().x());
    const int delta = vertical ? e->angleDelta().y() : e->angleDelta().x();

    if (vertical) {
        if (delta < 0) {
#else
    if (e->orientation() == Qt::Vertical) {
        if (e->delta() < 0) {
#endif
            stack.setCurrentIndex(stack.currentIndex() + 1);
        } else {
            stack.setCurrentIndex(stack.currentIndex() - 1);
        }
        emit PageButtonsUpdate();
    }
}

void CEmoticonsWidget::PageButtonsUpdate()
{
    QList<QRadioButton*> pageButtons = this->findChildren<QRadioButton*>(QString());
    foreach (QRadioButton* t_pageButton, pageButtons) {
        if (t_pageButton->property("pageIndex").toInt() == stack.currentIndex())
            t_pageButton->setChecked(true);
        else
            t_pageButton->setChecked(false);
    }
}

void CEmoticonsWidget::keyPressEvent(QKeyEvent* e)
{
    Q_UNUSED(e)
    hide();
}
