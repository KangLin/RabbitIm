#include "FrmGroupChatList.h"
#include "ui_FrmGroupChatList.h"
#include "../../Global/Global.h"
#include "FrmGroupChatFind.h"
#include "../../MainWindow.h"
#include "DlgCreateGroupChatRoom.h"
#include <QDesktopWidget>

#define ROLE_JID Qt::UserRole + 1
#define ROLE_FORM_GROUPCHAT ROLE_JID + 1

CFrmGroupChatList::CFrmGroupChatList(QWidget *parent) :
    QFrame(parent),
    m_GroupList(this),
    ui(new Ui::CFrmGroupChatList)
{
    ui->setupUi(this);

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);

    bool check = false;
    m_pAction = NULL;

    InitMenu();

    m_pModel = new QStandardItemModel(this);
    if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("Rooms")<< tr("Information"));
    }

    //禁止列表文本框编辑  
    m_GroupList.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_GroupList.setModel(m_pModel);
    m_GroupList.show();

    check = connect(&m_GroupList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);

    check = connect(&m_GroupList, SIGNAL(clicked(QModelIndex)),
                    SLOT(slotClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_GroupList, SIGNAL(doubleClicked(QModelIndex)),
                    SLOT(slotDoubleClicked(QModelIndex)));
    Q_ASSERT(check);
}

CFrmGroupChatList::~CFrmGroupChatList()
{
    if(m_pModel)
        delete m_pModel;

    delete ui;
}

void CFrmGroupChatList::resizeEvent(QResizeEvent* e)
{
    LOG_MODEL_DEBUG("CFrmGroupChatList", "CFrmGroupChatList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());
    m_GroupList.resize(this->geometry().size());

    //调整列的宽度  
    int nWidth = m_GroupList.geometry().width() * 4/ 5;
    m_GroupList.setColumnWidth(0, nWidth);
    m_GroupList.setColumnWidth(1, m_GroupList.geometry().width() - nWidth - 5);
}

void CFrmGroupChatList::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

int CFrmGroupChatList::InitMenu()
{
    m_Menu.setTitle(tr("Operator group chat(&G)"));
    m_Menu.addAction(ui->actionOpen_chat_room);
    m_Menu.addAction(ui->actionCreate_chat_room);
    m_Menu.addAction(ui->actionJoin_chat_room);
    m_Menu.addAction(ui->actionLeave_room);
    m_Menu.addAction(ui->actionRoom_infomation);

    bool check = connect(GET_MAINWINDOW, SIGNAL(sigMenuInitOperator(QMenu*)),
                    SLOT(slotAddToMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuRemoveOperator(QMenu*)),
                    SLOT(slotRemoveFromMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(&m_Menu, SIGNAL(aboutToShow()), SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    return 0;
}

void CFrmGroupChatList::slotAddToMainMenu(QMenu *pMenu)
{
    m_pAction = pMenu->addMenu(&m_Menu);
    bool check = connect(pMenu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));//slotUpdateMainMenu
    Q_ASSERT(check);
}

void CFrmGroupChatList::slotRemoveFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pAction);
    pMenu->disconnect(this);
}

void CFrmGroupChatList::slotUpdateMenu()
{
    if(this->isHidden())
    {
        m_Menu.setEnabled(false);
        return;
    }

    m_Menu.setEnabled(true);

    ui->actionCreate_chat_room->setVisible(true);
    ui->actionJoin_chat_room->setVisible(true);
    
    QString room = GetCurrentRoom();
    if(room.isEmpty())//列表是空  
    {
        ui->actionLeave_room->setVisible(false);
        ui->actionOpen_chat_room->setVisible(false);
        ui->actionRoom_infomation->setVisible(false);
        return;
    }

    ui->actionLeave_room->setVisible(true);
    ui->actionOpen_chat_room->setVisible(true);
    ui->actionRoom_infomation->setVisible(true);
}

void CFrmGroupChatList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    m_Menu.exec(QCursor::pos());
}

void CFrmGroupChatList::on_actionCreate_chat_room_triggered()
{
    CDlgCreateGroupChatRoom room;
    room.exec();
}

void CFrmGroupChatList::on_actionJoin_chat_room_triggered()
{
    
}

void CFrmGroupChatList::on_actionOpen_chat_room_triggered()
{
    
}

void CFrmGroupChatList::on_actionLeave_room_triggered()
{
    
}

void CFrmGroupChatList::slotClicked(const QModelIndex &index)
{
#ifdef MOBILE
    /*const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_OBJECT);
    CFrmGroupChat* chat = v.value<CFrmGroupChat*>();
    chat->show();*/
#endif
}

void CFrmGroupChatList::slotDoubleClicked(const QModelIndex &index)
{
#ifndef ANDROID
    /*const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_OBJECT);
    CFrmGroupChat* chat = v.value<CFrmGroupChat*>();
    chat->show();
    chat->activateWindow();*/
#endif
}

QString CFrmGroupChatList::GetCurrentRoom()
{
    QModelIndex index = m_GroupList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)//列表是空  
    {
       return QString();
    }

}
