#include "ManageMessageDialog.h"

CManageMessageDialog::CManageMessageDialog()
{
}

CManageMessageDialog::~CManageMessageDialog()
{
    Clean();
}

int CManageMessageDialog::Init(const QString &szId)
{
    return 0;
}

int CManageMessageDialog::Clean()
{
    m_DlgMessage.clear();
    return 0;
}

int CManageMessageDialog::ShowDialog(const QString &szId)
{
    QSharedPointer<CFrmMessage> frmMessage;
    QMap<QString, QSharedPointer<CFrmMessage> >::iterator it;
    it = m_DlgMessage.find(szId);
    if(m_DlgMessage.end() == it)
    {
        QSharedPointer<CFrmMessage> frm(new CFrmMessage(szId));
        frmMessage = frm;
        m_DlgMessage.insert(szId, frm);
    }
    else
        frmMessage = it.value();
    frmMessage->show();
    frmMessage->activateWindow();
    return 0;
}
