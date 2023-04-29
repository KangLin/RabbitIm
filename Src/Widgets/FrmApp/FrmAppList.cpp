#include "FrmAppList.h"
#include "ui_FrmAppList.h"
#include "../../Global/Global.h"
#include "RabbitCommonDir.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QSettings>

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
    InitList(); //在 changeEvent 中的 ui 语言改变事件中调用  
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
    SaveGroupNodeStateToStorage();
    delete ui;
}

QString CFrmAppList::GetNodeStateFile()
{
    QString szId;
    if(!GLOBAL_USER.isNull()
            && !USER_INFO_LOCALE.isNull())
    {
        szId = USER_INFO_LOCALE->GetInfo()->GetId();
    }
    if(szId.isEmpty())
        return QString();

    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(), QSettings::IniFormat);
    QString szLocale = conf.value("Global/Language", QLocale::system().name()).toString();
    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }
    return CGlobalDir::Instance()->GetDirUserData(szId)
            + QDir::separator() + "AppListGroupNodeState_" + szLocale + ".dat";
}

int CFrmAppList::LoadGroupNodeStateFromStorage()
{
    QString szFile = GetNodeStateFile();
    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        qWarning("Don't open file:%s",
                          szFile.toStdString().c_str());
        return -1;
    }

    QDataStream s(&in);
    try{
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        qDebug("Version:%d", nVersion);
        int nRowCount = 0;
        s >> nRowCount;
        while(nRowCount--)
        {
            //本地用户信息  
            QString szGroup;
            s >> szGroup;
            bool bState;
            s >> bState;
            qDebug("Group:%s; State:%d", szGroup.toStdString().c_str(), bState);
            QStandardItem* pItem = nullptr;
            QModelIndexList lstIndexs = m_pModel->match(
                        m_pModel->index(0, 0),
                        ROLE_GROUP,
                        szGroup,
                        -1,
                        Qt::MatchStartsWith
                        | Qt::MatchWrap
                        | Qt::MatchRecursive
                        | Qt::MatchCaseSensitive);
            if(lstIndexs.isEmpty())
            {
                pItem = ItemInsertGroup(szGroup);
                QModelIndex index = m_pModel->indexFromItem(pItem);
                if(bState)
                    m_AppList.expand(index);
                else
                    m_AppList.collapse(index);
            }else{                
                QModelIndex index;
                foreach(index, lstIndexs)
                {
                    pItem = m_pModel->itemFromIndex(index);
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
    }
    catch(...)
    {
        qCritical() << "CFrmAppList::LoadGroupNodeStateFromStorage exception";
        return -2;
    }

    in.close();  

    return 0;
}

int CFrmAppList::SaveGroupNodeStateToStorage()
{
    QString szFile = GetNodeStateFile();
    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        qWarning("Don't open file:%s",
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
            qDebug("text:%s",
                            item->text().toStdString().c_str());
            s << item->text()
              << m_AppList.isExpanded(index);
        }
    }
    catch(...)
    {
        qCritical() << "CFrmAppList::SaveGroupNodeStateToStorage exception";
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
                                  CPluginApp *app)
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

int CFrmAppList::InsertApp(CPluginApp *app)
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
    QSharedPointer<CManagePluginApp> plugin = GETMANAGER->GetManagePluginApp();
    if(plugin.isNull())
        return -1;
    m_pModel->clear();
    LoadGroupNodeStateFromStorage();
    std::list<CPluginApp* > lstPlugin = plugin->GetAllPlugins();
    std::list<CPluginApp* >::iterator it;
    for(it = lstPlugin.begin(); it != lstPlugin.end(); it++)
    {
        InsertApp(*it);
    }
    return nRet;
}

void CFrmAppList::clicked(const QModelIndex &index)
{
    qDebug("CFrmAppList::clicked, row:%d; column:%d",
           index.row(), index.column());

#ifdef MOBILE
    if(m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
    {
        qCritical("CFrmAppList::clicked model is null");   
        return;
    }

    slotOpenApp();
#endif
}

void CFrmAppList::doubleClicked(const QModelIndex &index)
{
    qDebug("CFrmAppList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef MOBILE
    if(!m_AppList.isExpanded(index))
       m_AppList.collapse(index);
    else
       m_AppList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
    {
        qCritical("CFrmAppList::doubleClicked model is null");
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
        qCritical("CFrmAppList::slotOpenApp model is null");
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
        qCritical("CFrmAppList::slotOpenApp App name is empty");
        return;
    }
    QSharedPointer<CManagePluginApp> plugin = GETMANAGER->GetManagePluginApp();
    if(plugin.isNull())
    {
        qCritical("CFrmAppList::slotOpenApp plugin is null");
        return;
    }
    plugin->GetPlugin(szApp)->Open(NULL, this);
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

    QSharedPointer<CManagePluginApp> plugin = GETMANAGER->GetManagePluginApp();
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

    QSharedPointer<CManagePluginApp> plugin = GETMANAGER->GetManagePluginApp();
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
    GETMANAGER->GetManagePluginApp()->AddFavority(szApp);
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

void CFrmAppList::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        InitList();
        break;
    }
}
