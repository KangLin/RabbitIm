#include "FrmContainer.h"
#include "ui_FrmContainer.h"
#include "FrmMessage.h"
#include "Global/Global.h"

CFrmContainer::CFrmContainer(QWidget *parent) :
    QFrame(parent),
    m_tabWidget(this),
    ui(new Ui::CFrmContainer)
{
    ui->setupUi(this);
    m_tabWidget.clear();
    m_tabWidget.setTabsClosable(true);
    bool check = connect(&m_tabWidget, SIGNAL(tabCloseRequested(int)),
                         SLOT(slotCloseTable(int)));
    Q_ASSERT(check);
    m_nSize = 10;
}

CFrmContainer::~CFrmContainer()
{
    LOG_MODEL_DEBUG("CFrmContainer", "CFrmContainer::~CFrmContainer()");
    m_tabWidget.clear();
    m_Frame.clear();
    delete ui;
}

int CFrmContainer::ShowDialog(const QString &szId)
{
    int nRet = -1;
    QMap<QString, QSharedPointer<QFrame> >::iterator it = m_Frame.find(szId);
    //找到,显示对话框  
    if(m_Frame.end() != it)
    {
        m_tabWidget.setCurrentWidget(it.value().data());
        m_tabWidget.activateWindow();
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

    //新建对话框,并添加到容器中  
    QSharedPointer<CUserInfo> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(!roster.isNull())
    {
        QSharedPointer<QFrame> frame(new CFrmMessage(szId, &m_tabWidget));
        QPixmap pixmap;
        pixmap.convertFromImage(roster->GetPhoto());
        int nIndex = m_tabWidget.addTab(frame.data(), QIcon(pixmap), roster->GetShowName());
        if(nIndex < 0)
        {
            LOG_MODEL_ERROR("CFrmContainer", "add tab fail");
            return -2;
        }

        m_tabWidget.setCurrentIndex(nIndex);
        m_tabWidget.activateWindow();
        m_tabWidget.show();
        m_Frame.insert(szId, frame);
        this->show();
        this->activateWindow();
        return 0;
    }

    //TODO:增加组对话框  

    return nRet;
}

int CFrmContainer::CloaseDialog(const QString &szId)
{
    QMap<QString, QSharedPointer<QFrame> >::iterator it = m_Frame.find(szId);
    if(m_Frame.end() != it)
    {
        m_tabWidget.setCurrentWidget(it.value().data());
        m_tabWidget.removeTab(m_tabWidget.currentIndex());
        m_Frame.erase(it);
        return 0;
    }
    return -1;
}

void CFrmContainer::resizeEvent(QResizeEvent *e)
{
    m_tabWidget.resize(this->geometry().size());
}

void CFrmContainer::closeEvent(QCloseEvent *)
{
    LOG_MODEL_DEBUG("CFrmContainer", "CFrmContainer::closeEvent");
}

void CFrmContainer::slotCloseTable(int nIndex)
{
    QFrame* frame = (QFrame*)m_tabWidget.widget(nIndex);
    m_tabWidget.removeTab(nIndex);
    QMap<QString, QSharedPointer<QFrame> >::iterator it;
    for(it = m_Frame.begin(); it != m_Frame.end(); it++)
    {
        if(it.value() == frame)
        {
            m_Frame.erase(it);
            break;
        }
    }
}
