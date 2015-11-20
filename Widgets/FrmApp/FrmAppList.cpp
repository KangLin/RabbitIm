#include "FrmAppList.h"
#include "ui_FrmAppList.h"
#include "../../Global/Global.h"
#include <QDir>
#include <QFile>
#include <QDataStream>

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
    m_AppList.setModel(m_pModel);
    InitMenu();
    //LoadGroupNodeStateFromStorage();
    InitList();
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
    //SaveGroupNodeStateToStorage();
    delete ui;
}

int CFrmAppList::LoadGroupNodeStateFromStorage()
{
    QString szId;
    if(!GLOBAL_USER.isNull()
            && !USER_INFO_LOCALE.isNull())
    {
        szId = USER_INFO_LOCALE->GetInfo()->GetId();
    }
    if(szId.isEmpty())
        return 0;
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId)
            + QDir::separator() + "AppListGroupNodeState.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        LOG_MODEL_WARNING("CFrmAppList", "Don't open file:%s",
                          szFile.toStdString().c_str());
        return -1;
    }

    QDataStream s(&in);
    try{
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        int nRowCount = 0;
        s >> nRowCount;
        while(nRowCount--)
        {
            //本地用户信息  
            LOG_MODEL_DEBUG("CFrmUserList", "Version:%d", nVersion);
            QString szGroup;
            s >> szGroup;
            ItemInsertGroup(szGroup);
            bool bState;
            s >> bState;
            QModelIndexList lstIndexs = m_pModel->match(
                        m_pModel->index(0, 0),
                        ROLE_GROUP,
                        szGroup,
                        -1,
                        Qt::MatchStartsWith
                        | Qt::MatchWrap
                        | Qt::MatchRecursive
                        | Qt::MatchCaseSensitive);
            QModelIndex index;
            foreach(index, lstIndexs)
            {
                QStandardItem* pItem = m_pModel->itemFromIndex(index);
                if(!pItem) continue;
                if(pItem->text() != szGroup)
                    continue;
                if(bState)
                    m_AppList.expand(index);
                else
                    m_AppList.collapse(index);
            }
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CFrmAppList",
               "CFrmAppList::LoadGroupNodeStateFromStorage exception");
        return -2;
    }

    in.close();  

    return 0;
}

int CFrmAppList::SaveGroupNodeStateToStorage()
{
    QString szId;
    if(!GLOBAL_USER.isNull()
            && !USER_INFO_LOCALE.isNull())
    {
        szId = USER_INFO_LOCALE->GetInfo()->GetId();
    }
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId)
            + QDir::separator() + "AppListGroupNodeState.dat";

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        LOG_MODEL_WARNING("CFrmAppList", "Don't open file:%s",
                          szFile.toStdString().c_str());
        return -3;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;

        int nRowCount = m_pModel->rowCount();
        s << nRowCount;
        for(int i = 0; i < nRowCount; i++)
        {
            QModelIndex index;
            index = m_pModel->index(i, 0);
            QStandardItem* item = m_pModel->itemFromIndex(index);
            LOG_MODEL_DEBUG("CFrmAppList", "text:%s",
                            item->text().toStdString().c_str());
            s << item->text()
              << m_AppList.isExpanded(index);
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CFrmAppList",
                "CFrmAppList::SaveGroupNodeStateToStorage exception");
        return -4;
    }

    out.close();
    return 0;
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
    pItemApp->setData(app->ID(), ROLE_APPLICATION);
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
    LOG_MODEL_DEBUG("CFrmAppList", "CFrmAppList::clicked, row:%d; column:%d",
           index.row(), index.column());

#ifdef MOBILE
    if(m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
    {
        LOG_MODEL_ERROR("CFrmAppList", "CFrmAppList::clicked model is null");   
        return;
    }

    slotOpenApp();
#endif
}

void CFrmAppList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("CFrmAppList", "CFrmAppList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef MOBILE
    if(!m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
    {
        LOG_MODEL_ERROR("CFrmAppList", "CFrmAppList::doubleClicked model is null");
        return;
    }

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
    {
        LOG_MODEL_ERROR("CFrmAppList", "CFrmAppList::slotOpenApp model is null");
        return;
    }

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, ROLE_APPLICATION);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
    {
        LOG_MODEL_ERROR("CFrmAppList", "CFrmAppList::slotOpenApp App name is empty");
        return;
    }
    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
    {
        LOG_MODEL_ERROR("CFrmAppList", "CFrmAppList::slotOpenApp plugin is null");
        return;
    }
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

void CFrmAppList::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //调整列的宽度  
    int nWidth = m_AppList.geometry().width();
    m_AppList.setColumnWidth(0, nWidth);
}
