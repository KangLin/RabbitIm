#include "MenuMessageSend.h"
#include "../../Global.h"

CMenuMessageSend::CMenuMessageSend(QWidget *parent) :
    QMenu(parent)
{
    m_ActionEnter = NULL;
    m_ActionCtrlEnter = NULL;
    InitMenu();
}

int CMenuMessageSend::InitMenu()
{
    m_ActionEnter = addAction(tr("Enter Send"), this, SLOT(slotActionEnter()));
    m_ActionCtrlEnter = addAction(tr("Ctrl + Enter Send"), this, SLOT(slotActionCtrlEnter()));
    m_ActionEnter->setCheckable(true);
    m_ActionCtrlEnter->setCheckable(true);
    bool check = connect(this, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));
    Q_ASSERT(check);
    return 0;
}

void CMenuMessageSend::slotActionCtrlEnter()
{
    CGlobal::Instance()->SetMessageSendType(CGlobal::E_MESSAGE_SEND_TYPE_CTRL_ENTER);
}

void CMenuMessageSend::slotActionEnter()
{
    CGlobal::Instance()->SetMessageSendType(CGlobal::E_MESSAGE_SEND_TYPE_ENTER);
}

void CMenuMessageSend::slotUpdateMenu()
{
    m_ActionCtrlEnter->setChecked(false);
    m_ActionEnter->setChecked(false);
    switch(CGlobal::Instance()->GetMessageSendType())
    {
    case CGlobal::E_MESSAGE_SEND_TYPE_CTRL_ENTER:
        m_ActionCtrlEnter->setChecked(true);
        break;
    case CGlobal::E_MESSAGE_SEND_TYPE_ENTER:
        m_ActionEnter->setChecked(true);
        break;
    }
}
