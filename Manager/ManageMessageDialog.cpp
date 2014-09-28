#include "ManageMessageDialog.h"
#include "Global/Global.h"

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
    QFrame* frmMessage;
    QMap<QString, QFrame*>::iterator it;
    it = m_DlgMessage.find(szId);
    if(m_DlgMessage.end() == it)
    {
        //是好友消息对话框  
        QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!roster.isNull())
        {
            CFrmMessage* frm = new CFrmMessage(szId);
            frmMessage = frm;
            bool check = connect(frm, SIGNAL(sigClose(QFrame*)),
                                 SLOT(slotDeleteFrmMessage(QFrame*)));
            Q_ASSERT(check);
            m_DlgMessage.insert(szId, frm);
        }
        else
        {
            //TODO:是组消息对话框  
            
        }
    }
    else
        frmMessage = it.value();
    frmMessage->show();
    frmMessage->activateWindow();
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
            m_DlgMessage.remove(it.key());
            delete pFrame;
            return;
        }
    }
}
