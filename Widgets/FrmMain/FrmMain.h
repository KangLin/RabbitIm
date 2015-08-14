#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QFrame>
#ifdef RABBITIM_WEBKIT
    #include <QWebView>
#endif
#include "../FrmUserList/FrmUserList.h"
#include "../FrmGroupChat/FrmGroupChatList.h"
#include "../FrmRecentMessage/FrmRecentMsgList.h"
#include "../../Lbs/FrmLbsMain.h"

namespace Ui {
class CFrmMain;
}

class MainWindow;
class CFrmMain : public QFrame
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit CFrmMain(QWidget *parent = 0);
    ~CFrmMain();

private:
    virtual void resizeEvent(QResizeEvent *e);
    virtual void changeEvent(QEvent*);
    virtual void showEvent(QShowEvent *);

private slots:
    //更新本地用户信息  
    void slotUpdateLocaleUserInfo();

#ifdef RABBITIM_WEBKIT
private slots:
    void slotWebViewLinkClicked(const QUrl &url);
private:
    QSharedPointer<QWebView> m_weather;
#endif

private:
    Ui::CFrmMain *ui;
    
    CFrmRecentMsgList m_MessageList;
    CFrmUserList m_UserList;
    CFrmGroupChatList m_GroupChatList;
    CFrmLbsMain m_Lbs;
};

#endif // FRMMAIN_H
