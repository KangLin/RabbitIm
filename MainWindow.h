#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "XmppClient.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"

class CFrmLogin;
class CFrmUserList;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CXmppClient* m_pClient;

protected slots:
    void About();
    void clientConnected();
    void clientDisconnected();
    void clientError(QXmppClient::Error e);
    void clientIqReceived(const QXmppIq &iq);
    void stateChanged(QXmppClient::State state);

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    CFrmLogin *m_pLogin;
    CFrmUserList* m_pUserList;
};

#endif // MAINWINDOW_H
