#include "FrmFavorites.h"
#include "ui_FrmFavorites.h"
#include "Global/Global.h"

CFrmFavorites::CFrmFavorites(QWidget *parent) :
    QWidget(parent),
    m_List(this),
    ui(new Ui::CFrmFavorites)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(&m_List);
    m_pModel = new QStandardItemModel(this);
    m_List.setModel(m_pModel);
    m_List.show();
    
    InitMenu();
    InitList();
    bool check = false;
    check = connect(GETMANAGER->GetManagePlugins().data(),
                    SIGNAL(sigAddFavority(QString)),
                    SLOT(slotAddFavorites(QString)));
    Q_ASSERT(check);
   /* check = connect(GETMANAGER->GetManagePlugins().data(),
                    SIGNAL(sigRemoveFavority(QString)),
                    SLOT(slotRemoveFavorites(QString)));
    Q_ASSERT(check);*/
    check = connect(&m_List, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_List, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);
    check = connect(&m_List, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);
}

CFrmFavorites::~CFrmFavorites()
{
    delete ui;
}

int CFrmFavorites::InitList()
{
    std::list<QString> lstFavorites = GETMANAGER->GetManagePlugins()->GetFavorites();
    std::list<QString>::iterator it;
    for(it = lstFavorites.begin(); it != lstFavorites.end(); it++)
    {
        slotAddFavorites(*it);
    }
    return 0;
}

void CFrmFavorites::slotAddFavorites(const QString &szApp)
{       
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                            Qt::DisplayRole, 
                                            szApp, 
                                            -1,
                                            Qt::MatchStartsWith
                                            | Qt::MatchWrap 
                                            | Qt::MatchRecursive
                                            | Qt::MatchCaseSensitive);
    if(!lstIndexs.isEmpty())
    {
        return;
    }
    
    QStandardItem* lstGroup = NULL;
    QSharedPointer<CPluginApp> app =
            GETMANAGER->GetManagePlugins()->GetPlugin(szApp);
    if(app.isNull())
        return;
    lstGroup = new QStandardItem(app->Icon(), szApp);
    lstGroup->setEditable(false);  //禁止双击编辑  
    m_pModel->appendRow(lstGroup);
    return ;
}

void CFrmFavorites::slotRemoveFavorites(const QString &szApp)
{
    QStandardItem* pItem = NULL;        
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                            Qt::DisplayRole, 
                                            szApp, 
                                            -1,
                                            Qt::MatchStartsWith
                                            | Qt::MatchWrap 
                                            | Qt::MatchRecursive
                                            | Qt::MatchCaseSensitive);
    if(!lstIndexs.isEmpty())
    {
        return ;
    }
    
    return ;
}


int CFrmFavorites::InitMenu()
{
    m_Menu.addAction(ui->actionOpen_Application);
    connect(ui->actionOpen_Application, SIGNAL(triggered()),
            SLOT(slotOpenApp()));
    m_Menu.addAction(ui->actionApplication_Infomation);
    connect(ui->actionApplication_Infomation, SIGNAL(triggered()),
            SLOT(slotAboutApp()));
    m_Menu.addAction(ui->actionRemove_Application);
    connect(ui->actionRemove_Application, SIGNAL(triggered()),
            SLOT(slotRemoveApp()));
    return 0;
}

void CFrmFavorites::slotOpenApp()
{
    QModelIndex index = m_List.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, Qt::DisplayRole);
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


void CFrmFavorites::slotCloseApp()
{
    QModelIndex index = m_List.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, Qt::DisplayRole);
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

void CFrmFavorites::slotAboutApp()
{
    QModelIndex index = m_List.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, Qt::DisplayRole);
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

void CFrmFavorites::slotRemoveApp()
{
    QModelIndex index = m_List.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, Qt::DisplayRole);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;

    m_pModel->removeRow(index.row());

    QSharedPointer<CManagePlugin> plugin = GETMANAGER->GetManagePlugins();
    if(plugin.isNull())
        return;
    plugin->RemoveFavority(szApp);
}

void CFrmFavorites::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QModelIndex index = m_List.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //是用户结点  
    QString szApp;
    QVariant v = m->data(index, Qt::DisplayRole);
    if(v.canConvert<QString>())
    {
        szApp = v.value<QString>();
    }
    if(szApp.isEmpty())
        return;
    m_Menu.exec(QCursor::pos());
}

void CFrmFavorites::clicked(const QModelIndex &index)
{
#ifdef MOBILE
    if(m_List.isExpanded(index))
       m_List.collapse(index);
    else
       m_List.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    slotOpenApp();
#endif
}

void CFrmFavorites::doubleClicked(const QModelIndex &index)
{
#ifndef MOBILE
    if(!m_List.isExpanded(index))
       m_List.collapse(index);
    else
       m_List.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    slotOpenApp();
#endif
}
