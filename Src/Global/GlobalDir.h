#ifndef CGLOBALDIR_H
#define CGLOBALDIR_H

#include <QString>

#if WIN32 && BUILD_SHARED_LIBS
    #ifdef DLL_EXPORT
        #define RABBITIM_SHARED_LIBRARY __declspec(dllexport)
    #else
        #define RABBITIM_SHARED_LIBRARY __declspec(dllimport)
    #endif
#else
    #define RABBITIM_SHARED_LIBRARY
#endif

/**
 * @ingroup RABBITIM_GLOBAL
 */
class RABBITIM_SHARED_LIBRARY CGlobalDir
{
public:
    CGlobalDir();
    
    static CGlobalDir* Instance();

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
     * @brief 得到表情包目录
     * @return 
     */
    QString GetDirEmoji();

};

#endif // CGLOBALDIR_H
