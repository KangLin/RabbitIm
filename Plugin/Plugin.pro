#RABBITIM_USE_PLUGIN_APP=1 #生成插件应用程序  

TEMPLATE = subdirs

lbs_app.SUBDIRS = Lbs
lbs_app.file = Lbs/Lbs_App.pro

QRCode_App.SUBDIRS = QRCode
QRCode_App.file = QRCode/QRCode_App.pro

SUBDIRS += QRCode Lbs 

equals(RABBITIM_USE_PLUGIN_APP, 1) {
    SUBDIRS += QRCode_App lbs_app
}
