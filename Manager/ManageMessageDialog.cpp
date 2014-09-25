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
    m_DlgMessage.clear();
    return 0;
}

int CManageMessageDialog::ShowDialog(const QString &szId)
{
    QSharedPointer<QFrame> frmMessage;
    QMap<QString, QSharedPointer<QFrame> >::iterator it;
    it = m_DlgMessage.find(szId);
    if(m_DlgMessage.end() == it)
    {
        QSharedPointer<QFrame> frm(new CFrmMessage(szId));
        frmMessage = frm;
        m_DlgMessage.insert(szId, frm);
    }
    else
        frmMessage = it.value();
    frmMessage->show();
    frmMessage->activateWindow();
    return 0;
}

int CManageMessageDialog::CloaseDialog(const QString &szId)
{
    return !m_DlgMessage.remove(szId);
}
