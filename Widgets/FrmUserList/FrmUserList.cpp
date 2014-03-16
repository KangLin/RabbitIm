#include "FrmUserList.h"
#include "ui_FrmUserList.h"

CFrmUserList::CFrmUserList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);//TODO:这里是否会产生内在泄漏？
    ui->tvUsers->setModel(m_pModel);
    ui->tvUsers->setHeaderHidden(true);
}

CFrmUserList::~CFrmUserList()
{
    delete ui;
}

int CFrmUserList::InsertUser(QString szJid, QString szUser, QSet<QString> szGroups)
{
    int nRet = 0;

    for(QSet<QString>::iterator it = szGroups.begin(); it != szGroups.end(); it++)
    {
        QString szGroup = *it;
        QList<QStandardItem*> lstGroup = m_pModel->findItems(szGroup);
        if(lstGroup.begin() != lstGroup.end())
        {
            QStandardItem* item = new QStandardItem(szJid);
            (*lstGroup.begin())->appendRow(item);
        }
        else
        {
            QStandardItem* item = new QStandardItem(szJid);
            QStandardItem* group = new QStandardItem(szGroup);
            group->appendRow(item);
            m_pModel->appendRow(group);
        }
    }

    return nRet;
}

int CFrmUserList::RemoveUser(QString szJid)
{
    int nRet = 0;

    return nRet;
}
