#include "DlgUservCard.h"
#include "ui_DlgUservCard.h"
#include "../../Global/Global.h"
#include <QFileDialog>
#include <QImageWriter>
#include <QDesktopWidget>

CDlgUservCard::CDlgUservCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUservCard)
{
    ui->setupUi(this);
    Init();
    m_bModify = false;
}

CDlgUservCard::CDlgUservCard(const QString &jid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUservCard)
{
    ui->setupUi(this);

    Init();

    m_bModify = false;
    m_szJid = jid;

    bool check = connect(GET_CLIENT.data(),
                         SIGNAL(sigUpdateRosterUserInfo(QString,QSharedPointer<CUser>)),
                         SLOT(slotUpdateRoster(QString, QSharedPointer<CUser>)));
    Q_ASSERT(check);

    GET_CLIENT->RequestUserInfoRoster(jid);
}

CDlgUservCard::CDlgUservCard(QSharedPointer<CUserInfo> user, bool bModify, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgUservCard),
    m_UserInfo(user),
    m_bModify(bModify)
{
    ui->setupUi(this);
    Init();
}

CDlgUservCard::~CDlgUservCard()
{
    LOG_MODEL_DEBUG("CDlgUservCard", "CDlgUservCard::~CDlgUservCard");
    GET_CLIENT.data()->disconnect(this);
    delete ui;
}

int CDlgUservCard::Init()
{  
    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->availableGeometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif
    return 0;
}

void CDlgUservCard::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("CDlgUservCard", "CDlgUservCard::showEvent");

    if(m_UserInfo.isNull())
        return;

    ui->txtJID->setText(m_UserInfo->GetId());
    ui->txtName->setText(m_UserInfo->GetName());
    ui->txtNick->setText(m_UserInfo->GetNick());
    ui->dateBirthday->setDate(m_UserInfo->GetBirthday());
    ui->txtEmail->setText(m_UserInfo->GetEmail());
    ui->txtDescription->setText(m_UserInfo->GetDescription());

    QImageWriter imageWriter(&m_Buffer, "png");
    m_Buffer.open(QIODevice::WriteOnly);
    if(!imageWriter.write(m_UserInfo->GetPhoto()))
        LOG_MODEL_ERROR("CDlgUservCard", "error:%s", imageWriter.errorString().toStdString().c_str());
    m_Buffer.close();

    QPixmap pixmap;
    pixmap.convertFromImage(m_UserInfo->GetPhoto());
    ui->lbPhoto->setPixmap(pixmap);
    ui->lbPhoto->setScaledContents(true);

    ui->pbBrower->setVisible(m_bModify);
    ui->pbClear->setVisible(m_bModify);
    ui->pbOK->setVisible(m_bModify);
}

void CDlgUservCard::closeEvent(QCloseEvent *)
{
}

void CDlgUservCard::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgUservCard::on_pbBrower_clicked()
{
    //*从资源中加载应用程序样式  
    QString szFile = QFileDialog::getOpenFileName(
                this, tr("Open File"), 
                QString(), "*.png", 0,
                QFileDialog::ReadOnly /*| QFileDialog::DontUseNativeDialog*/);
    if(szFile.isEmpty())
       return; 

    QImageWriter imageWriter(&m_Buffer, "png");
    m_Buffer.open(QIODevice::WriteOnly);
    if(!imageWriter.write(QImage(szFile)))
        LOG_MODEL_ERROR("CDlgUservCard", "error:%s", imageWriter.errorString().toStdString().c_str());
    m_Buffer.close();

    ui->lbPhoto->setPixmap(QPixmap(szFile));
}

void CDlgUservCard::on_pbClear_clicked()
{
    ui->lbPhoto->clear();
    m_Buffer.setData(NULL, 0);
}

void CDlgUservCard::on_pbOK_clicked()
{
    QSharedPointer<CUserInfo> userInfo = USER_INFO_LOCALE->GetInfo();
    userInfo->SetName(ui->txtName->text());
    userInfo->SetNick(ui->txtNick->text());
    userInfo->SetBirthday(ui->dateBirthday->date());
    userInfo->SetEmail(ui->txtEmail->text());
    userInfo->SetDescription(ui->txtDescription->text());
    userInfo->SetPhoto(m_Buffer.buffer());
    GET_CLIENT->setlocaleUserInfo(userInfo);
    close();
}

void CDlgUservCard::on_pbCancel_clicked()
{
    close();
}

void CDlgUservCard::slotUpdateRoster(const QString& szId, QSharedPointer<CUser> userInfo)
{
    if(szId == m_szJid)
    {
        if(m_UserInfo.isNull())
        {
            m_UserInfo = userInfo->GetInfo();
        }
        showEvent(NULL);
    }
}
