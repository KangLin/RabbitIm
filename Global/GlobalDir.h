#ifndef CGLOBALDIR_H
#define CGLOBALDIR_H

#include <QString>

class CGlobalDir
{
public:
    CGlobalDir();
    
    static CGlobalDir* Instance();
    
    /// 应用程序目录  
    QString GetDirApplication();
    /// 文档目录，默认是系统文档目录  
    QString GetDirDocument();
    int SetDirDocument(QString szPath);
    /// 应用程序配置目录  
    //QString GetDirApplicationConfigure();
    /// 应用程序数据目录  
    //QString GetDirApplicationData();
    /// 应用程序下载目录  
    QString GetDirApplicationDownLoad();
    /// 运动数据目录  
    QString GetDirMotion(const QString &szId = QString());
    
    /**
     * @brief 用户配置目录  
     *
     * @param szId:本地用户id,默认为本地用户   
     */
    QString GetDirUserConfigure(const QString &szId = QString());
    /**
     * @brief 用户数据存放目录  
     *
     * @param szId:本地用户id,默认为本地用户   
     */
    QString GetDirUserData(const QString &szId = QString());
    /**
     * @brief 用户临时目录  
     * @param szId
     * @return 
     */
    QString GetDirTempDir(const QString &szId = QString());
    
    /// 翻译文件目录  
    QString GetDirTranslate();
    /// 应用程序配置文件  
    QString GetApplicationConfigureFile();
    /**
     * @brief 得到用户的配置文件  
     *
     * @param szId:本地用户id,默认为本地用户   
     */
    QString GetUserConfigureFile(const QString &szId = QString());
    /**
     * @brief 得到用户头像目录  
     *
     * @param szId:本地用户id,默认为本地用户   
     */
    QString GetDirUserAvatar(const QString &szId = QString());
    /**
     * @brief 得到指定用户的头像文件  
     *
     * @param szId:好友的ID   
     * @param szLocalId:本地用户的ID,如果为空,则会是登录用户  
     */
    QString GetFileUserAvatar(const QString &szId, const QString &szLocalId = QString());
    /**
     * @brief 得到接收文件保存的目录  
     *
     * @param szId:本地用户id,默认为本地用户   
     */
    QString GetDirReceiveFile(const QString &szId = QString());
    
    /** 
     * 得到表情图片文件  
     */
    QString GetFileSmileyPack();
private:
    QString m_szDocumentPath;
};

#endif // CGLOBALDIR_H
