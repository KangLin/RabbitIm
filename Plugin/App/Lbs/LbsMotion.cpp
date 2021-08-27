#include "LbsMotion.h"
#include "ui_LbsMotion.h"
#include <QNmeaPositionInfoSource>
#include <QQuickItem>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include "LbsCamera.h"
#include "Global/GlobalDir.h"
#include "RabbitCommonLog.h"

CLbsMotion::CLbsMotion(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CLbsMotion),
    m_MessageBox(this)
{
    ui->setupUi(this);
    //加载地图到 quickWidge    
    ui->qwMap->setSource(QStringLiteral("qrc:///qml/Map.qml"));
    
    ui->lbDistance->setText(tr("%1\nDistance(km)").arg("0.00"));
    ui->lbDistance_1->setText(tr("%1\nDistance(km)").arg("0.00"));
    ui->lbDistance_1->setVisible(false);
    ui->lbDuration->setText(tr("%1\nDuration").arg("00:00:00"));
    ui->lbAccuracy->setText(tr("%1\nAccuracy(m)").arg("0.00"));
    ui->lbSpeed->setText(tr("%1\nSpeed(km/h)").arg("0.00"));
    ui->lbRealTimeSpeed->setText(tr("%1\nReal Time Speed(m/s)").arg("0.00"));
    ui->pbPause->setVisible(false);
    ui->pbCamera->setVisible(false);
    ui->pbShow->setVisible(false);

    m_bStart = false;
    m_bPause = false;
    m_bHide = false;
    m_Source = NULL;
    m_pLogger = NULL;
    m_StartTime = 0;
    m_PauseTime = 0;
    m_Distance = 0;
    m_pUploadThread = NULL;
    
    //*默认 opents gprmc 上传   
    m_szUrl = "http://182.254.185.29:8080/gprmc/Data";
    m_szUser = "root";
    m_szDevice = "123456";//*/

#ifdef MOBILE
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
#else
    //*
    QNmeaPositionInfoSource* pSource = new QNmeaPositionInfoSource(
                QNmeaPositionInfoSource::SimulationMode, this);
    m_Source = pSource;
    if(m_Source)
    {
        m_inFile.setFileName("D:/temp/gps.nmea");
        if(m_inFile.open(QIODevice::ReadOnly))
        {   
            pSource->setDevice(&m_inFile);
            //p->setUpdateInterval(1000);
        }
        else
            LOG_MODEL_ERROR("CLbsTrack", "don't open file:%s",
                            m_inFile.fileName().toStdString().c_str());
    }//*/    
#endif

    bool check = false;
    if (m_Source) {
        check = connect(m_Source,
                        SIGNAL(positionUpdated(QGeoPositionInfo)),
                        SLOT(positionUpdated(QGeoPositionInfo)));
        Q_ASSERT(check);
        
        //在地图上显示轨迹  
        QQuickItem * pRoot = ui->qwMap->rootObject();
        QObject* pMap = pRoot->findChild<QObject*>("objNameMap");
        if(pMap)
        {
            QGeoPositionInfo pos = m_Source->lastKnownPosition();
            pMap->setProperty("center", QVariant::fromValue(pos.coordinate()));
        }
    }
    else
        LOG_MODEL_ERROR("CLbsMotion", "QGeoPositionInfoSource is null");

    check = connect(&m_Timer, SIGNAL(timeout()),
                         SLOT(OnTimeOut()));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigExitMessageBox(int)),
                    SLOT(slotExitMessageBox(int)));
    Q_ASSERT(check);
    CLbsCamera* pCamera = CLbsCamera::Instance();
    if(pCamera)
    {
        check = connect(pCamera, SIGNAL(sigPhotograph(QString)),
                        SLOT(slotPhotograph(QString)));
        Q_ASSERT(check);
    }   
}

CLbsMotion::~CLbsMotion()
{
    LOG_MODEL_DEBUG("CLbsMotion", "~CLbsMotion");
    if(m_bStart)
        on_pbStart_clicked();

    CLbsCamera* p = CLbsCamera::Instance();
    if(p)
        p->disconnect(this);

    if(m_pUploadThread && m_pUploadThread->joinable())
    {
        m_pUploadThread->join();
        delete m_pUploadThread;
        m_pUploadThread = NULL;
    }

    delete ui;
    
}

void CLbsMotion::positionUpdated(const QGeoPositionInfo &info)
{
    if(!info.isValid())
    {
        LOG_MODEL_ERROR("CLbsMotion", "QGeoPositionInfo is invalid\n");
        return;
    }

    if(!m_bStart || m_bPause)
    {
        m_lastPostion = QGeoPositionInfo();
        return;
    }

    //保存到本地轨迹文件中  
    if(m_pLogger)
        m_pLogger->Write(info);

    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
    {
        ui->lbAccuracy->setText(tr("%1\nAccuracy(m)").arg(QString::number(
            info.attribute(QGeoPositionInfo::HorizontalAccuracy), 'f', 3)));
    }
    if(info.hasAttribute(QGeoPositionInfo::GroundSpeed))
    {
        ui->lbRealTimeSpeed->setText(tr("%1\nReal Time Speed(m/s)").arg(
                                   QString::number(info.attribute(
                                   QGeoPositionInfo::GroundSpeed), 'f', 3)));
    }

    //在地图上显示轨迹  
    QQuickItem * pRoot = ui->qwMap->rootObject();
    QObject* pMap = pRoot->findChild<QObject*>("objNameMap");
    if(pMap)
    {
        pMap->setProperty("center", QVariant::fromValue(info.coordinate()));
        QMetaObject::invokeMethod(pMap, "addPolylinePoint",
                   Q_ARG(QVariant, QVariant::fromValue(info.coordinate())));
    }

    //计算距离  
    if(m_lastPostion.isValid() && m_lastPostion.coordinate().isValid())
    {
        m_Distance += info.coordinate().distanceTo(m_lastPostion.coordinate());
        //LOG_MODEL_DEBUG("CLbsMotion", "distanceTo:%f", info.coordinate().distanceTo(m_lastPostion.coordinate()));
    }
    m_lastPostion = info;
}

void CLbsMotion::on_pbStart_clicked()
{
    if(NULL == m_Source)
    {
        return;
    }

    if(m_bStart) //停止  
    {
        m_Timer.stop();
        m_Source->stopUpdates();
        ui->pbStart->setText(tr("Start"));
        ui->pbPause->setVisible(false);
        ui->pbCamera->setVisible(false);
        ui->pbShow->setVisible(false);

        if(m_pLogger)
        {
            m_pLogger->CloseFile();
            delete m_pLogger;
            m_pLogger = NULL;
        }

        m_MessageBox.setWindowTitle(tr("Upload"));
        m_MessageBox.setText(tr("Upload to server ......"));
        m_MessageBox.setStandardButtons(QMessageBox::NoButton);
        //上传到服务器  
        if(m_pUploadThread)
        {
            m_pUploadThread->join();
            delete m_pUploadThread;
            m_pUploadThread = NULL;
        }
        m_pUploadThread = new std::thread(onRunUpload, this);
        m_MessageBox.exec();
    }
    else //开始  
    {
        if(NULL == m_pLogger)
        {
            m_szSaveFile = CGlobalDir::Instance()->GetDirMotion();
            QDir d;
            if(!d.exists(m_szSaveFile))
                d.mkpath(m_szSaveFile);
            m_szSaveFile += QDir::separator()
                    + QDateTime::currentDateTime().toString("yyyyMMddHHmmss")
                    + ".dat";
            m_pLogger = new CLbsPositionLogger(
                        m_szSaveFile.toStdString().c_str());
            if(m_pLogger)
                m_pLogger->OpenFile();
        }
 
        m_PauseTime = 0;
        m_Distance = 0;
        m_StartTime = QDateTime::currentDateTime().toTime_t();
        m_Timer.start(1000);

        ui->pbStart->setText(tr("Stop"));
        ui->pbPause->setText(tr("Pause"));
        ui->pbPause->setVisible(true);
        ui->pbCamera->setVisible(true);
        ui->pbShow->setVisible(true);

        //初始化地图  
        QQuickItem * pRoot = ui->qwMap->rootObject();
        QObject* pMap = pRoot->findChild<QObject*>("objNameMap");
        if(pMap)
        {
            QMetaObject::invokeMethod(pMap, "init");
        }

        m_Source->startUpdates();
    }
    m_bStart = !m_bStart;
    m_bPause = false;
}

void CLbsMotion::on_pbPause_clicked()
{
    if(m_bPause)
        m_StartTime += QDateTime::fromTime_t(m_PauseTime).secsTo(
                    QDateTime::currentDateTime());
    m_PauseTime = QDateTime::currentDateTime().toTime_t();
    m_bPause = !m_bPause;
    if(m_bPause)
        ui->pbPause->setText(tr("Continue"));
    else
        ui->pbPause->setText(tr("Pause"));
}

void CLbsMotion::on_pbShow_clicked()
{
    /*QRect startRect, endRect;
    startRect = ui->wdgPrompt->geometry();
    */

    m_bHide = !m_bHide;
    if(m_bHide)
    {
        ui->pbShow->setIcon(QIcon(":/png/down"));
    }
    else
    {
        ui->pbShow->setIcon(QIcon(":/png/up"));
    }
    if(m_bStart)
        ui->pbPause->setVisible(!m_bHide);
    ui->pbStart->setVisible(!m_bHide);
    ui->lbDistance->setVisible(!m_bHide);
    ui->lbDistance_1->setVisible(m_bHide);
    ui->lbSpeed->setVisible(!m_bHide);
    ui->lbAccuracy->setVisible(!m_bHide);
    ui->lbRealTimeSpeed->setVisible(!m_bHide);

    /*
    endRect = ui->wdgPrompt->geometry();
    m_Animation.setStartValue(startRect);
    m_Animation.setEndValue(endRect);
    m_Animation.setDuration(1000);
    m_Animation.start();*/
}

void CLbsMotion::OnTimeOut()
{
    if(m_bPause)
        return;
    long sec = QDateTime::fromTime_t(m_StartTime).secsTo(
                QDateTime::currentDateTime());
    QTime t(0, 0, 0);
    t = t.addSecs(sec);
    ui->lbDuration->setText(tr("%1\nDuration").arg(t.toString("HH:mm:ss")));
    ui->lbDistance->setText(tr("%1\nDistance(km)").arg(
                                QString::number(m_Distance / 1000, 'f', 3)));
    ui->lbDistance_1->setText(tr("%1\nDistance(km)").arg(
                                QString::number(m_Distance / 1000, 'f', 3)));
    ui->lbSpeed->setText(tr("%1\nSpeed(km/h)").arg(
                          QString::number(m_Distance * 3.6 / sec , 'f', 3)));
    /*LOG_MODEL_DEBUG("CLbsMotion", "time:%d;distance:%f;speed:%f",
                    sec,
                    m_Distance,
                    m_Distance / sec);*/
}

int CLbsMotion::onRunUpload(void *pThis)
{
    CLbsMotion* p = (CLbsMotion*)pThis;

    p->onUploadServer();
    //因为对话框在主线程中显示需要时间,  
    //所以在没有数据要上传时,休眠等对话框显示    
    QThread::msleep(500);
    return 0;
}

int CLbsMotion::onUploadServer()
{
    int nRet = 0;

    CLbsPositionLogger up(m_szSaveFile.toStdString().c_str());
    nRet = up.UploadServer(m_szUrl, m_szUser, "", m_szDevice);

    emit sigExitMessageBox(nRet);
    return nRet;
}

void CLbsMotion::slotExitMessageBox(int nRet)
{
    LOG_MODEL_DEBUG("CLbsMotion", "CLbsMotion::slotExitMessageBox, nRet=%d", nRet);
    if(0 == nRet)
    {
        m_MessageBox.setText(tr("Upload succeed"));
        m_MessageBox.accept();
        return ;
    }

    m_MessageBox.setText(tr("Upload fail"));
    m_MessageBox.setStandardButtons(QMessageBox::Yes);
}

void CLbsMotion::on_pbCamera_clicked()
{
    CLbsCamera* p = CLbsCamera::Instance();
    if(p)
        p->slotOpen();
    LOG_MODEL_DEBUG("CLbsMotion", "on_pbCamera_clicked");
}

void CLbsMotion::slotPhotograph(const QString &szFile)
{
    LOG_MODEL_DEBUG("CLbsMotion", "slotPhotograph:%s", szFile.toStdString().c_str());
}

void CLbsMotion::on_pbBack_clicked()
{
    if(m_bStart)
        on_pbStart_clicked();
    emit sigClose();
}
