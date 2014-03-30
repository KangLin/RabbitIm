#ifndef FRMVIDEO_H
#define FRMVIDEO_H

#include <QFrame>
#include "../../XmppClient.h"
#include <QAudioOutput>
#include <QAudioInput>
#include <QBuffer>

class CRoster;

namespace Ui {
class CFrmVideo;
}

class CFrmVideo : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmVideo(QWidget *parent = 0);
    ~CFrmVideo();

    int SetClient(CXmppClient *pClient);
    int Call(QString jid);

protected slots:
    void callReceived(QXmppCall* pCall);
    void callStarted(QXmppCall *pCall);

    /// \brief This signal is emitted when the remote party is ringing.
    void ringing();

    /// \brief This signal is emitted when the call state changes.
    void stateChanged(QXmppCall::State state);

    /// \brief This signal is emitted when a call is connected.
    ///
    /// Once this signal is emitted, you can connect a QAudioOutput and
    /// QAudioInput to the call. You can determine the appropriate clockrate
    /// and the number of channels by calling payloadType().
    void connected();

    /// \brief This signal is emitted when the audio channel changes.
    void audioModeChanged(QIODevice::OpenMode mode);

    /// \brief This signal is emitted when the video channel changes.
    void videoModeChanged(QIODevice::OpenMode mode);

    /// \brief This signal is emitted when a call is finished.
    ///
    /// Note: Do not delete the call in the slot connected to this signal,
    /// instead use deleteLater().
    void finished();

private:
    Ui::CFrmVideo *ui;

    QXmppCall* m_pCall;
    CXmppClient *m_pClient;

    QAudioInput*        m_pAudioInput;
    QAudioOutput*       m_pAudioOutput;
};

#endif // FRMVIDEO_H
