#include "FrmAppList.h"
#include "ui_FrmAppList.h"
#include "../../Global/Global.h"

CFrmAppList::CFrmAppList(QWidget *parent) :
    QWidget(parent),
    m_AppList(this),
    ui(new Ui::CFrmAppList)
{
    ui->setupUi(this);

    ui->gridLayout->addWidget(&m_AppList);
    m_pModel = new QStandardItemModel(this);
    /*if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("group"));
    }*/

    InitMenu();
    InitList();
    m_AppList.setModel(m_pModel);
    m_AppList.show();
    
    bool check = connect(&m_AppList, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_AppList, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_AppList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);
}

CFrmAppList::~CFrmAppList()
{
    delete ui;
}

QStandardItem* CFrmAppList::ItemInsertGroup(QString szGroup)
{
    QStandardItem* lstGroup = NULL;
    lstGroup = new QStandardItem(QIcon(":/icon/Application"), szGroup);
    lstGroup->setEditable(false);  //禁止双击编辑  
    lstGroup->setData(szGroup, ROLE_GROUP);
    m_pModel->appendRow(lstGroup);
    return lstGroup;
}

int CFrmAppList::InsertAppInGroup(QStandardItem *pGroupItem,
                                  QSharedPointer<CPluginApp> app)
{
    /*在插件注册时，就已经去掉重复，所以这里就不用检查了  
    QModelIndex index = m_pModel->indexFromItem(pGroupItem);
    QModelIndexList lstIndexs = m_pModel->match(index,
                                            ROLE_GROUP, 
                                            app->Name(), 
                                            -1,
                                            Qt::MatchStartsWith
                                            | Qt::MatchCaseSensitive);
    if(!lstIndexs.isEmpty())
    {
        LOG_MODEL_DEBUG("CFrmAppList",
                        "Application[%s] is existed", app->Name());
        return -1;
    }*/
    QStandardItem* pItemApp = NULL;
    pItemApp = new QStandardItem(app->Icon(), app->Name());
    pItemApp->setEditable(false);  //禁止双击编辑  
    pItemApp->setData(app->Name(), ROLE_APPLICATION);
    pGroupItem->appendRow(pItemApp);
    return 0;
}

int CFrmAppList::InsertApp(QSharedPointer<CPluginApp> app)
{
    int nRet = 0;
    QSet<QString> group = app->Group();
    QString szGroup;
    foreach(szGroup, group)
    {
        QStandardItem* pItem = NULL;        
        QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                ROLE_GROUP, 
                                                szGroup, 
                                                -1,
                                                Qt::MatchStartsWith
                                                | Qt::MatchWrap 
                                                | Qt::MatchRecursive
                                                | Qt::MatchCaseSensitive);
        if(lstIndexs.isEmpty())
        {
            //插入新组  
            pItem = ItemInsertGroup(szGroup);
            InsertAppInGroup(pItem, app);
        }
        else
        {
            QModelIndex index;
            foreach(index, lstIndexs)
            {
                pItem = m_pModel->itemFromIndex(index);
                InsertAppInGroup(pItem, app);
            }
        }
    }
    return nRet;
}

int CFrmAppList::InitList()
{
    int nRet = 0;
    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
        return -1;
    std::list<QSharedPointer<CPluginApp> > lstPlugin = plugin->GetAllPlugins();
    std::list<QSharedPointer<CPluginApp> >::iterator it;
    for(it = lstPlugin.begin(); it != lstPlugin.end(); it++)
    {
        InsertApp(*it);
    }
    return nRet;
}

void CFrmAppList::clicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::clicked, row:%d; column:%d",
           index.row(), index.column());

#ifdef MOBILE
    if(m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    slotOpenApp();
#endif
}

void CFrmAppList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef MOBILE
    if(!m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    slotOpenApp();
#endif
}

int CFrmAppList::InitMenu()
{
    m_Menu.addAction(ui->actionOpen_Application);
    connect(ui->actionOpen_Application, SIGNAL(triggered()),
            SLOT(slotOpenApp()));
    m_Menu.addAction(ui->actionCollect_Application);
    connect(ui->actionCollect_Application, SIGNAL(triggered(bool)),
            SLOT(slotCollectApp()));
    m_Menu.addAction(ui->actionApplication_Infomation);
    connect(ui->actionApplication_Infomation, SIGNAL(triggered()),
            SLOT(slotAboutApp()));
    return 0;
}

void CFrmAppList::slotOpenApp()
{
    QModelIndex index = m_AppList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;

    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
        return;
    plugin->GetPlugin(szApp)->Open(this);
}


void CFrmAppList::slotCloseApp()
{
    QModelIndex index = m_AppList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;

    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
        return;
    plugin->GetPlugin(szApp)->Close();
}

void CFrmAppList::slotAboutApp()
{
    QModelIndex index = m_AppList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;

    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
        return;
    plugin->GetPlugin(szApp)->About();
}

void CFrmAppList::slotCollectApp()
{
    QModelIndex index = m_AppList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;
  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;
    //增加到收藏中  
    GETMANAGER->GetManagePlugins()->AddFavority(szApp);
}

void CFrmAppList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QModelIndex index = m_AppList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;
    m_Menu.exec(QCursor::pos());
}
