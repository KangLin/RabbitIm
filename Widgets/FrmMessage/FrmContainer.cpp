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
    m_nSize = 10;
}

CFrmContainer::~CFrmContainer()
{
    LOG_MODEL_DEBUG("CFrmContainer", "CFrmContainer::~CFrmContainer()");
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
    return !m_Frame.remove(szId);
}

void CFrmContainer::resizeEvent(QResizeEvent *e)
{
    m_tabWidget.resize(this->geometry().size());
}

void CFrmContainer::closeEvent(QCloseEvent *)
{
}
