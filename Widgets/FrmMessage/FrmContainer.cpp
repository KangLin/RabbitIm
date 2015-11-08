#include "FrmContainer.h"
#include "ui_FrmContainer.h"
#include "FrmMessage.h"
#include "Global/Global.h"
#include "MainWindow.h"
#include "Widgets/FrmGroupChat/FrmGroupChat.h"

CFrmContainer::CFrmContainer(QWidget *parent) :
    QFrame(parent),
    m_tabWidget(this),
    ui(new Ui::CFrmContainer)
{
    ui->setupUi(this);
    m_nSize = 10;
    m_bClose = 0;
    m_tabWidget.clear();
    //m_tabWidget.setTabPosition(QTabWidget::West);//设置标签位置  
    m_tabWidget.setTabsClosable(true);
    bool check = connect(&m_tabWidget, SIGNAL(tabCloseRequested(int)),
                         SLOT(slotCloseTable(int)));
    Q_ASSERT(check);

    check = connect(&m_tabWidget, SIGNAL(currentChanged(int)),
            SLOT(slotCurrentChanged(int)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotRefresh()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateRosterUserInfo(QString,QSharedPointer<CUser>)),
                    SLOT(slotUpdateRoster(QString,QSharedPointer<CUser>)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigChangedStatus(const QString&)),
                    SLOT(SlotChangedStatus(const QString&)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRemoveRosterUserInfo(QString)),
                    SLOT(slotRemove(QString)));
    Q_ASSERT(check);

    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigLeave(QString)),
                    SLOT(slotRemove(QString)));
    Q_ASSERT(check);

    QDesktopWidget *pDesk = QApplication::desktop();    
#ifdef MOBILE
    this->resize(pDesk->geometry().size());
#else
    move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);
#endif 
}

CFrmContainer::~CFrmContainer()
{
    LOG_MODEL_DEBUG("CFrmContainer", "CFrmContainer::~CFrmContainer()");
    GET_CLIENT->disconnect(this);
    GETMANAGER->GetManageGroupChat()->disconnect(this);
    CGlobal::Instance()->GetMainWindow()->disconnect(this);
    m_tabWidget.clear();
    QMap<QString, QFrame*>::iterator it;
    for(it = m_Frame.begin(); it != m_Frame.end(); it++)
    {
        delete it.value();
    }
    m_Frame.clear();
    delete ui;
}

int CFrmContainer::ShowDialog(const QString &szId)
{
    int nRet = -1;
    QMap<QString, QFrame* >::iterator it = m_Frame.find(szId);
    //找到,显示对话框  
    if(m_Frame.end() != it)
    {
        m_tabWidget.setCurrentWidget(it.value());
        //m_tabWidget.activateWindow();
        m_tabWidget.show();
        this->show();
        this->activateWindow();
        return 0;
    }

    //到达容器最大容量,返回  
    if(m_Frame.size() >= m_nSize)
    {
        return -1;
    }

    QFrame* pFrame = NULL;
    int nIndex = 0;
    //新建对话框,并添加到容器中  
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(!roster.isNull())
    {
        QSharedPointer<CUserInfo> info = roster->GetInfo();
        pFrame = new CFrmMessage(szId, &m_tabWidget);
        QPixmap pixmap;
        MainWindow::ComposeAvatarStatus(info, pixmap);
        nIndex = m_tabWidget.addTab(pFrame, QIcon(pixmap), info->GetShowName());
        if(nIndex < 0)
        {
            LOG_MODEL_ERROR("CFrmContainer", "add tab fail");
            return -2;
        }
    }
    else
    {
        //增加组对话框  
        QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
        if(gc.isNull())
        {
            LOG_MODEL_ERROR("CFrmContainer", "Don't group chat:%s", qPrintable(szId));
            return -3;
        }
        pFrame = new CFrmGroupChat(szId, &m_tabWidget);
        QString szText = gc->ShowName();
        if(!gc->Subject().isEmpty())
            szText += ":" + gc->Subject();
        nIndex = m_tabWidget.addTab(pFrame, gc->Icon(), szText);
        if(nIndex < 0)
        {
            LOG_MODEL_ERROR("CFrmContainer", "add tab fail");
            return -2;
        }
    }
    bool check = connect(pFrame, SIGNAL(sigClose(QFrame*)),
                         SLOT(slotDeleteFrame(QFrame*)));
    Q_ASSERT(check);

    m_tabWidget.setCurrentIndex(nIndex);
    //m_tabWidget.activateWindow();
    m_tabWidget.show();
    m_Frame.insert(szId, pFrame);
    this->show();
    this->activateWindow();
    return 0;

    return nRet;
}

void CFrmContainer::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    m_tabWidget.resize(this->geometry().size());
}

void CFrmContainer::closeEvent(QCloseEvent *e)
{
    LOG_MODEL_DEBUG("CFrmContainer", "CFrmContainer::closeEvent");
    m_bClose = true;
    QMap<QString, QFrame* >::iterator it, oldIt;
    it = m_Frame.begin();
    while(m_Frame.end() != it && oldIt != it)
    {
        oldIt = it;
        it.value()->close();
        it = m_Frame.begin();
        if(oldIt == it)
        {
            e->ignore();
            return;
        }
    }
    emit sigClose(this);
}

void CFrmContainer::slotDeleteFrame(QFrame *frame)
{
    int nOldIndex = m_tabWidget.currentIndex();
    if(-1 == nOldIndex)
        return;
    m_tabWidget.setCurrentWidget(frame);
    int nIndex = m_tabWidget.currentIndex();
    m_tabWidget.setCurrentIndex(nOldIndex);
    
    m_tabWidget.removeTab(nIndex);
    QMap<QString, QFrame* >::iterator it;
    for(it = m_Frame.begin(); it != m_Frame.end(); it++)
    {
        if(it.value() == frame)
        {
            delete *it;
            m_Frame.erase(it);
            break;
        }
    }

    if(!m_Frame.isEmpty())
    {
        return;
    }

    if(!m_bClose)
    {
        //如果没有子窗口了，则不是close事件，通知容器窗口删除掉自己  
        emit sigClose(this);
    }
}

void CFrmContainer::slotRemove(const QString &szId)
{
    QMap<QString, QFrame*>::iterator it;
    it = m_Frame.find(szId);
    if(m_Frame.end() == it)
    {
        return;
    }

    slotDeleteFrame(it.value());
}

void CFrmContainer::slotCloseTable(int nIndex)
{
    QFrame* frame = (QFrame*)m_tabWidget.widget(nIndex);
    frame->close();
    return;
}

void CFrmContainer::slotCurrentChanged(int index)
{
    QFrame* frame = (QFrame*)m_tabWidget.widget(index);
    if(!frame)
    {
        return;
    }
    this->setWindowIcon(m_tabWidget.tabIcon(index));
    this->setWindowTitle(m_tabWidget.tabText(index));
}

void CFrmContainer::SlotChangedStatus(const QString &szId)
{
    Q_UNUSED(szId);
    slotRefresh();
}

void CFrmContainer::slotUpdateRoster(const QString& szId, QSharedPointer<CUser> user)
{
    int nIndex = m_tabWidget.currentIndex();
    QMap<QString, QFrame* >::iterator it;
    for(it = m_Frame.begin(); it != m_Frame.end(); it++)
    {
        QString id = it.key();
        if(id != szId)
            continue;
        //是好友消息对话框  
        QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!roster.isNull())
        {
            m_tabWidget.setCurrentWidget(it.value());
            int index = m_tabWidget.currentIndex();
            if(-1 == index)
            {
                LOG_MODEL_ERROR("CFrmContainer", "There isn't the widget");
                continue;
            }
            QSharedPointer<CUserInfo> info = roster->GetInfo();
            QPixmap pixmap;
            MainWindow::ComposeAvatarStatus(info, pixmap);
            m_tabWidget.setTabIcon(index, QIcon(pixmap));
            m_tabWidget.setTabText(index, info->GetShowName());
            continue;
        }
    }
    m_tabWidget.setCurrentIndex(nIndex);
}

void CFrmContainer::slotRefresh()
{
    int nIndex = m_tabWidget.currentIndex();
    QMap<QString, QFrame* >::iterator it;
    for(it = m_Frame.begin(); it != m_Frame.end(); it++)
    {
        QString szId = it.key();
        //是好友消息对话框  
        QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!roster.isNull())
        {
            m_tabWidget.setCurrentWidget(it.value());
            int index = m_tabWidget.currentIndex();
            if(-1 == index)
            {
                LOG_MODEL_ERROR("CFrmContainer", "There isn't the widget");
                continue;
            }
            QSharedPointer<CUserInfo> info = roster->GetInfo();
            QPixmap pixmap;
            MainWindow::ComposeAvatarStatus(info, pixmap);
            m_tabWidget.setTabIcon(index, QIcon(pixmap));
            m_tabWidget.setTabText(index, info->GetShowName());
            continue;
        }
        //TODO:是组消息对话框  
        
    }
    //当当前是自己时，setCurrentIndex 不会触发信号currentChanged  
    slotCurrentChanged(nIndex);
    m_tabWidget.setCurrentIndex(nIndex);
}
