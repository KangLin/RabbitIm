#include "ManageMessageDialog.h"
#include "Global/Global.h"
#include "Widgets/FrmGroupChat/FrmGroupChat.h"

CManageMessageDialog::CManageMessageDialog(QObject *parent) : QObject(parent)
{
}

CManageMessageDialog::~CManageMessageDialog()
{
    LOG_MODEL_DEBUG("CManageMessageDialog", "CManageMessageDialog::~CManageMessageDialog()");
    Clean();
}

int CManageMessageDialog::Init(const QString &szId)
{
    Q_UNUSED(szId);
    return 0;
}

int CManageMessageDialog::Clean()
{
    LOG_MODEL_DEBUG("CManageMessageDialog", "CManageMessageDialog::Clean()");
    QMap<QString, QFrame*>::iterator it;
    for(it = m_DlgMessage.begin(); it != m_DlgMessage.end(); it++)
    {
        delete it.value();
    }
    m_DlgMessage.clear();
    return 0;
}

int CManageMessageDialog::ShowDialog(const QString &szId)
{
    QFrame* pFrame = NULL;
    QMap<QString, QFrame*>::iterator it;
    it = m_DlgMessage.find(szId);
    if(m_DlgMessage.end() == it)
    {
        //是好友消息对话框  
        QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!roster.isNull())
        {
            pFrame = new CFrmMessage(szId);
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
            pFrame = new CFrmGroupChat(szId);
        }

        if(!pFrame)
        {
            LOG_MODEL_ERROR("CFrmContainer", "pFrame is null");
            return -4;
        }
        bool check = connect(pFrame, SIGNAL(sigClose(QFrame*)),
                             SLOT(slotDeleteFrmMessage(QFrame*)));
        Q_ASSERT(check);
        m_DlgMessage.insert(szId, pFrame);
    }
    else
        pFrame = it.value();

    if(pFrame)
    {
        pFrame->show();
        pFrame->activateWindow();
    }
    return 0;
}

void CManageMessageDialog::slotDeleteFrmMessage(QFrame *obj)
{
    QMap<QString, QFrame*>::iterator it;
    for(it = m_DlgMessage.begin(); it != m_DlgMessage.end(); it++)
    {
        if(it.value() == obj)
        {
            QFrame* pFrame = it.value();
            m_DlgMessage.erase(it);
            delete pFrame;
            return;
        }
    }
}

int CManageMessageDialog::Show()
{
    QMap<QString, QFrame*>::iterator it;
    for(it = m_DlgMessage.begin(); it != m_DlgMessage.end(); it++)
    {
        it.value()->show();
    }
    return 0;
}

int CManageMessageDialog::Hide()
{
    QMap<QString, QFrame*>::iterator it;
    for(it = m_DlgMessage.begin(); it != m_DlgMessage.end(); it++)
    {
        it.value()->hide();
    }
    return 0;
}
