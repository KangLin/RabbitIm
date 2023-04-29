#include "CallObject.h"
#include "Global/Global.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Widgets/FrmVideo/CameraQtCaptureVideoFrame.h"

#include <QCameraInfo>

CCallObject::CCallObject(const QString &szId, bool bVideo, QObject *parent) :
    QObject(parent)
{
    m_nError = 0;
    m_szId = szId;
    SetDirection(IncomingDirection);
    m_pSound = nullptr;
    m_bVideo = bVideo;
    m_pFrmVideo = nullptr;    
    m_pCamera = nullptr;
    m_pAudioInput = nullptr;
    m_pAudioOutput = nullptr;
    
    m_CaptureVideoFrame = QSharedPointer<CCameraQtCaptureVideoFrame>(new CCameraQtCaptureVideoFrame());
    bool check = connect(CGlobal::Instance(), SIGNAL(sigVideoCaptureDevice(int)),
                         this, SLOT(slotChangeCamera(int)));
    Q_ASSERT(check);
    
    slotChanageState(CallState);
}

CCallObject::~CCallObject()
{
    qDebug("CCallObject::~CCallObject.id:%d", qPrintable(m_szId));
    StopCallSound();
    CloseDevices();
}

int CCallObject::Call()
{
    SetDirection(OutgoingDirection);
    return 0;
}

int CCallObject::SetError(int nError, const QString &szError)
{
    m_nError = nError;
    m_szError = szError;
    return 0;
}

int CCallObject::GetError(QString &szError)
{
    szError = m_szError;
    return m_nError;
}

QString CCallObject::GetId()
{
    Q_ASSERT(!m_szId.isEmpty());//请在派生类构造函数中设置此值  
    return m_szId;
}

int CCallObject::SetId(const QString szId)
{
    m_szId = szId;
    return 0;
}

int CCallObject::SetDirection(Direction d)
{
    m_Direction = d;
    return 0;
}

CCallObject::State CCallObject::GetState()
{
    return m_State;
}

CCallObject::Direction CCallObject::GetDirection()
{
    return m_Direction;
}

bool CCallObject::IsVideo()
{
    return m_bVideo;
}

//播放铃音,系统会启用单独的线程进行播放  
void CCallObject::PlayCallSound()
{
    if(IsMonitor())
        return;
    //qDebug("CCallObject::PlayCallSound");
    QString file;
    if(GetDirection() == CCallObject::OutgoingDirection)
        file = ":/sound/Call";
    else
        file = ":/sound/Receive";

    StopCallSound();
    m_pSound = new QSound(file);
    if(m_pSound)
    {
        m_pSound->setLoops(20);
        m_pSound->play();
    }
}

void CCallObject::StopCallSound()
{
    //qDebug("CCallObject::StopCallSound");
    if(m_pSound)
    {
        m_pSound->stop();
        delete m_pSound;
        m_pSound = nullptr;
    }
}

int CCallObject::OpenDevices()
{
    int nRet = 0;
    OpenVideoWindow();
    OpenCamera();

    return nRet;
}

int CCallObject::CloseDevices()
{
    int nRet = 0;
    CloseCamera();
    CloseVideoWindow();
    return nRet;
}

int CCallObject::OpenVideoWindow()
{
    if(IsMonitor() || !IsVideo())
        return -1;

    //防止重复打开  
    if(m_pFrmVideo)
    {
        qWarning("Video window is opened");
        return -2;
    }

    //打开显示对话框  
    m_pFrmVideo = new CFrmVideo();
    if(!m_pFrmVideo)
    {
        return -3;
    }

    CTool::EnableWake();
    QSharedPointer<CUser> roster
            = GLOBAL_USER->GetUserInfoRoster(this->GetId());
    if(roster.isNull())
    {
        qWarning("The roster is null");
        return false;
    }
    m_pFrmVideo->setWindowTitle(
           tr("Be talking with %1").arg(roster->GetInfo()->GetShowName()));
    m_pFrmVideo->setWindowIcon(QIcon(roster->GetInfo()->GetPhotoPixmap()));
    //窗口关闭时会自己释放内存  
    m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);

    bool check = connect(m_pFrmVideo, SIGNAL(destroyed()),
                         SLOT(slotFrmVideoClose()));
    Q_ASSERT(check);
    check = connect(m_CaptureVideoFrame.data(),
                    SIGNAL(sigCaptureFrame(const QImage &)),
                    this,  SIGNAL(sigRenderLocale(const QImage&)));
    Q_ASSERT(check);
    check = connect(m_CaptureVideoFrame.data(),
                    SIGNAL(sigCaptureFrame(const QVideoFrame &)),
                    this,  SLOT(soltVideoFrameToRemote(const QVideoFrame&)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigRenderLocale(const QImage&)),
                    m_pFrmVideo, SLOT(slotDisplayLocaleVideo(const QImage&)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigRenderRemote(const QImage&)),
                    m_pFrmVideo, SLOT(slotDisplayRemoteVideo(const QImage&)));
    Q_ASSERT(check);

    m_pFrmVideo->show();
    m_pFrmVideo->activateWindow();

    return 0;
}

int CCallObject::CloseVideoWindow()
{
    bool bCheck = false;
    bCheck = m_CaptureVideoFrame->disconnect();

    CTool::EnableWake(false);

    if(m_pFrmVideo)
    {
        this->disconnect(m_pFrmVideo);
        //因为在 OpenVideoWindow 设置了窗口关闭时会自己释放内存
        //m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);
        //所以这里不需要释放内存  
        m_pFrmVideo->close();
        m_pFrmVideo = nullptr;
    }
    return 0;
}

void CCallObject::slotFrmVideoClose()
{
    if(m_pFrmVideo)
    {
        //因为在 OpenVideoWindow 设置了窗口关闭时会自己释放内存
        //m_pFrmVideo->setAttribute(Qt::WA_DeleteOnClose, true);
        //所以这里不需要释放内存
        m_pFrmVideo = nullptr;

        if(FinishedState != m_State)
            this->Stop();
    }
}

void CCallObject::slotChanageState(CCallObject::State state)
{
    m_State = state;
    emit sigUpdate();
    switch(m_State)
    {
    case CallState:
        PlayCallSound();
        break;
    case ConnectingState:
        StopCallSound();
        break;
    case ActiveState:
        StopCallSound();
        OpenDevices();
        break;
    case DisconnectingState:
        break;
    case FinishedState:
        StopCallSound();
        CloseDevices();
        emit sigFinished(this);
        break;
    default:
        break;        
    }

    return;
}

bool CCallObject::IsMonitor()
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(this->GetId());
    if(roster.isNull())
    {
        qWarning("roster is null");
        return false;
    }
    if(GetDirection() == CCallObject::IncomingDirection
            && roster->GetInfo()->GetIsMonitor()
            && CGlobal::Instance()->GetIsMonitor())
    {
        return true;
    }
    return false;
}

int CCallObject::OpenCamera()
{
    int nRet = 0;

    QList<QCameraInfo> info = QCameraInfo::availableCameras();
    if(info.isEmpty())
        return -1;

    Q_ASSERT(!m_pCamera);
    m_pCamera = new QCamera(QCameraInfo::availableCameras().value(
                                CGlobal::Instance()->GetVideoCaptureDevice()),
                            this);
    if(!m_pCamera) return -2;
    m_pCamera->setViewfinder(m_CaptureVideoFrame.data());
    m_pCamera->start();

    return nRet;
}

int CCallObject::CloseCamera()
{
    int nRet = 0;
    if(!m_pCamera) return -1;
    m_pCamera->stop();
    m_pCamera->unload();
    delete m_pCamera;
    m_pCamera = nullptr;
    return nRet;
}

void CCallObject::slotChangeCamera(int nIndex)
{
    Q_UNUSED(nIndex)
    if(!m_pCamera) return;
    CloseCamera();
    OpenCamera();
}

int CCallObject::OpenAudioDevice(QAudioFormat inFormat,
                                 QAudioFormat outFormat,
                                 QIODevice *outDevice)
{
    int nRet = 0;

    QList<QAudioDeviceInfo> lstInputs 
            = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!lstInputs.isEmpty()
            && (lstInputs.size() > CGlobal::Instance()->GetAudioInputDevice())
            && CGlobal::Instance()->GetAudioInputDevice() > -1)
    {
        QAudioDeviceInfo infoAudioInput(lstInputs.value(CGlobal::Instance()->GetAudioInputDevice()));
        if (!infoAudioInput.isFormatSupported(inFormat)) {
            qWarning("Default audio input format not supported - trying to use nearest");
            //TODO:增加格式转换
            inFormat = infoAudioInput.nearestFormat(inFormat);
        }
        m_pAudioInput = new QAudioInput(infoAudioInput, inFormat, this);
        if(!m_pAudioInput)
            qCritical("Create QAudioInput device instance fail.");
        else if( (outDevice->openMode() & QIODevice::WriteOnly)
                && (m_pAudioInput->state() != QAudio::ActiveState) )
            m_pAudioInput->start(outDevice);
    }

    QList<QAudioDeviceInfo> lstOutputs
            = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    if(!lstOutputs.isEmpty()
            && (lstOutputs.size() > CGlobal::Instance()->GetAudioOutputDevice())
            && CGlobal::Instance()->GetAudioOutputDevice() > -1)
    {
        QAudioDeviceInfo infoAudioOutput(lstOutputs.value(CGlobal::Instance()->GetAudioOutputDevice()));
        if (!infoAudioOutput.isFormatSupported(outFormat)) {
            qWarning("Default audio output format not supported - trying to use nearest");
            //TODO:增加格式转换
            outFormat = infoAudioOutput.nearestFormat(outFormat);
        }
        m_pAudioOutput = new QAudioOutput(infoAudioOutput, outFormat, this);
        if(!m_pAudioOutput)
            qCritical("Create QAudioOutput device instance fail.");
        else if((outDevice->openMode() & QIODevice::ReadOnly) && (m_pAudioOutput->state() != QAudio::ActiveState) )
            m_pAudioOutput->start(outDevice);
    }
    return nRet;
}

int CCallObject::CloseAudioDevice()
{
    if(m_pAudioInput)
    {
        m_pAudioInput->stop();
        delete m_pAudioInput;
        m_pAudioInput = NULL;
    }

    if(m_pAudioOutput)
    {
        m_pAudioOutput->stop();
        delete m_pAudioOutput;
        m_pAudioOutput = NULL;
    }

    return 0;
}
