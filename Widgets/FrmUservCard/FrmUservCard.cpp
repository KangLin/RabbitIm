#include "FrmUservCard.h"
#include "ui_FrmUservCard.h"
#include "../../Global.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include <QFileDialog>
#include <QImageWriter>

CFrmUservCard::CFrmUservCard(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmUservCard)
{
    ui->setupUi(this);

    m_pRoster = NULL;
    m_pClient = NULL;
    m_bModify = false;
}

CFrmUservCard::CFrmUservCard(CRoster *pRoster, bool bModify, CXmppClient *pClient, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmUservCard),
    m_pRoster(pRoster),
    m_bModify(bModify),
    m_pClient(pClient)
{
    ui->setupUi(this);
}

CFrmUservCard::~CFrmUservCard()
{
    LOG_MODEL_DEBUG("CFrmUservCard", "CFrmUservCard::~CFrmUservCard");
    delete ui;
}

void CFrmUservCard::showEvent(QShowEvent *)
{
    LOG_MODEL_DEBUG("CFrmUservCard", "CFrmUservCard::showEvent");

    ui->txtJID->setText(m_pRoster->BareJid());
    ui->txtName->setText(m_pRoster->Name());
    ui->txtNick->setText(m_pRoster->Nick());
    ui->dateBirthday->setDate(m_pRoster->Birthday());
    ui->txtEmail->setText(m_pRoster->Email());
    ui->txtDescription->setText(m_pRoster->Description());

    QImageWriter imageWriter(&m_Buffer, "png");
    m_Buffer.open(QIODevice::WriteOnly);
    if(!imageWriter.write(m_pRoster->Photo()))
        LOG_MODEL_ERROR("CFrmUservCard", "error:%s", imageWriter.errorString().toStdString().c_str());
    m_Buffer.close();

    QPixmap pixmap;
    pixmap.convertFromImage(m_pRoster->Photo());
    ui->lbPhoto->setPixmap(pixmap);
    ui->lbPhoto->setScaledContents(true);

    ui->pbBrower->setVisible(m_bModify);
    ui->pbClear->setVisible(m_bModify);
    ui->pbOK->setVisible(m_bModify);
}

void CFrmUservCard::closeEvent(QCloseEvent *)
{
    deleteLater();
}

void CFrmUservCard::on_pbBrower_clicked()
{
    //*从资源中加载应用程序样式 
    QString szFile = QFileDialog::getOpenFileName(
                this, tr("Open File"), 
                QString(), "*.png", 0,
                QFileDialog::ReadOnly | QFileDialog::DontUseNativeDialog);
    if(szFile.isEmpty())
       return; 

    QImageWriter imageWriter(&m_Buffer, "png");
    m_Buffer.open(QIODevice::WriteOnly);
    if(!imageWriter.write(QImage(szFile)))
        LOG_MODEL_ERROR("CFrmUservCard", "error:%s", imageWriter.errorString().toStdString().c_str());
    m_Buffer.close();

    ui->lbPhoto->setPixmap(QPixmap(szFile));
}

void CFrmUservCard::on_pbClear_clicked()
{
    ui->lbPhoto->clear();
    m_Buffer.setData(NULL, 0);
}

void CFrmUservCard::on_pbOK_clicked()
{
    if(m_pClient)
    {
        QXmppVCardIq vCard;
        vCard.setFullName(ui->txtName->text());
        vCard.setNickName(ui->txtNick->text());
        vCard.setBirthday(ui->dateBirthday->date());
        vCard.setEmail(ui->txtEmail->text());
        vCard.setDescription(ui->txtDescription->text());
        vCard.setPhoto(m_Buffer.buffer());
        m_pClient->vCardManager().setClientVCard(vCard);
        m_pClient->vCardManager().requestClientVCard();
    }
    close();
}

void CFrmUservCard::on_pbCancel_clicked()
{
    close();
}
