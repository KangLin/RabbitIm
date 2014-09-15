#ifndef GLOBALUSER_H
#define GLOBALUSER_H

#include <QObject>
#include <QSharedPointer>
#include "UserInfo/UserInfo.h"
#include "UserInfo/COperateRoster.h"
#include "Client/Client.h"

class CGlobalUser : public QObject
{
    Q_OBJECT
public:
    explicit CGlobalUser(QObject *parent = 0);

    virtual ~CGlobalUser();

    virtual int Init(QString szLocaleJid);
    virtual int Clean();
    //设置内容修改标志  
    virtual int SetModify(bool bModify = true);

    //得到本地用户对象  
    virtual QSharedPointer<CUserInfo> GetUserInfoLocale();

    //处理好友列表操作  
    virtual int ProcessRoster(COperateRoster* pOperateRoster, void *para = NULL);
    //得到好友对象  
    //成功,返回好友对象指针.失败,返回空指针  
    virtual QSharedPointer<CUserInfo> GetUserInfoRoster(const QString &szId);
    //从好友列表中删除好友  
    virtual int RemoveUserInfoRoster(const QString &szId);

private:
    int LoadFromFile(QString szLocaleJid);
    int SaveToFile();
    int LoadLocaleFromFile(const QString &szLocaleJid);
    int SaveLocaleToFile();
    int LoadRosterFromFile(QString szLocaleJid);
    int SaveRosterToFile();

    //得到保存本地用户信息的文件  
    virtual QString GetLocaleFile(const QString &szLocaleJid);
    //得到保存好友信息的文件  
    virtual QString GetRosterFile(const QString &szLocaleJid);
    //新建立一个空的用户信息对象  
    virtual QSharedPointer<CUserInfo> NewUserInfo();

protected:
    //本地用户信息  
    QSharedPointer<CUserInfo> m_UserInforLocale;
    //好友信息列表  
    QMap<QString, QSharedPointer<CUserInfo> > m_UserInfoRoster;

protected:
    bool m_bModify; //标志内容是否修改  
    QSharedPointer<CClient> m_Client; 

signals:

public slots:

};

#endif // GLOBALUSER_H
