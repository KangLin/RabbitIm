#ifndef CPLUGINQRCODE_H
#define CPLUGINQRCODE_H

#include "../PluginApp.h"
#include "FrmScanQRcode.h"
#include <QObject>

class CPluginQRCode : public QObject,  CPluginApp
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID rabbitim_plugin_application_iid FILE "QRCode.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(CPluginApp)

public:
    CPluginQRCode(QObject *parent = 0);

    //登录初始化  
    virtual int Init(const QString &szId);
    //登出清理  
    virtual int Clean();

    ///操作  
    //打开应用  
    virtual int Open(void *pPara = 0, QWidget* parent = 0);
    //关闭应用  
    virtual int Close();

    ///属性  
    ///应用ID,应用以这个ID区分  
    virtual QString ID();
    //应用名称  
    virtual QString Name();
    //应用所在组  
    virtual QSet<QString> Group();
    //应用图标  
    virtual QIcon Icon();

private:
    QSharedPointer<CFrmScanQRcode> m_FrmScanQRCode;
};

#endif // CPLUGINQRCODE_H
