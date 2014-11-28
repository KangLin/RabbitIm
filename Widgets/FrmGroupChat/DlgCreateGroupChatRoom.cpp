#include "DlgCreateGroupChatRoom.h"
#include "ui_DlgCreateGroupChatRoom.h"
#include <QMessageBox>
#include <QDesktopWidget>

CDlgCreateGroupChatRoom::CDlgCreateGroupChatRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgCreateGroupChatRoom)
{
    ui->setupUi(this);

    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->geometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif
}

CDlgCreateGroupChatRoom::~CDlgCreateGroupChatRoom()
{
    delete ui;
}

void CDlgCreateGroupChatRoom::showEvent(QShowEvent *)
{
    ui->txtConfirmPassword->clear();
    ui->txtName->clear();
    ui->txtPassword->clear();
    ui->txtSubject->clear();
    ui->txtDescription->clear();
    ui->cbPrivated->setChecked(false);
    ui->cbProtracted->setChecked(false);
}

void CDlgCreateGroupChatRoom::closeEvent(QCloseEvent *)
{
}

void CDlgCreateGroupChatRoom::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgCreateGroupChatRoom::on_pbOK_clicked()
{
    QString szName = ui->txtName->text();
    if(szName.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room name."));
        return;
    }
    if(szName.indexOf(QRegExp("[^a-zA-Z0-9]")) != -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Room name can only be letters or numbers."));
        return;
    }
    if(ui->txtSubject->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room subject."));
        return;
    }

    if(ui->txtConfirmPassword->text() != ui->txtPassword->text())
    {
        QMessageBox::critical(this, tr("Error"), tr("Enter the password is not the same twice."));
        return;
    }
/*
    CFrmGroupChat* pGroupChat = new CFrmGroupChat();
    if(pGroupChat)
    {
        bool check = connect(pGroupChat, SIGNAL(sigJoined(const QString&,CFrmGroupChat*)),
                             SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)));
        Q_ASSERT(check);

        QXmppDataForm form(QXmppDataForm::Submit);
        QList<QXmppDataForm::Field> fields;
        {
            QXmppDataForm::Field field(QXmppDataForm::Field::HiddenField);
            field.setKey("FORM_TYPE");
            field.setValue("http://jabber.org/protocol/muc#roomconfig");
            fields.append(field);
        }
        QXmppDataForm::Field field;
        field.setKey("muc#roomconfig_roomname");
        field.setValue(ui->txtName->text());
        fields.append(field);
        field.setKey("muc#roomconfig_subject");
        field.setValue(ui->txtSubject->text());
        fields.append(field);
        field.setKey("muc#roomconfig_roomdesc");
        field.setValue(ui->txtDescription->text());
        fields.append(field);
        if(!ui->txtPassword->text().isEmpty())
        {
            field.setKey("muc#roomconfig_roomsecret");
            field.setValue(ui->txtPassword->text());
            fields.append(field);
            QXmppDataForm::Field field1(QXmppDataForm::Field::BooleanField);
            field1.setKey("muc#roomconfig_passwordprotectedroom");
            field1.setValue(true);
            fields.append(field1);
        }
        if(ui->cbPrivated->isChecked())
        {
            QXmppDataForm::Field field(QXmppDataForm::Field::BooleanField);
            field.setKey("muc#roomconfig_membersonly");
            field.setValue(true);
            fields.append(field);
        }
        if(ui->cbProtracted->isChecked())
        {
            QXmppDataForm::Field field(QXmppDataForm::Field::BooleanField);
            field.setKey("muc#roomconfig_persistentroom");
            field.setValue(true);
            fields.append(field);
        }
        form.setFields(fields);
        pGroupChat->setConfiguration(form);

        if(!pGroupChat->Create(szName + "@" + m_szId))
            delete pGroupChat;
    }
*/
    this->accept();
}

void CDlgCreateGroupChatRoom::on_pbCancel_clicked()
{
    this->reject();
}
