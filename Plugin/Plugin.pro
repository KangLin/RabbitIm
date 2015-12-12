#RABBITIM_USE_PLUGIN_APP=1 #生成插件应用程序  

TEMPLATE = subdirs

SUBDIRS += QRCode Lbs ScreenShot 

equals(RABBITIM_USE_PLUGIN_APP, 1) {
    for(app, SUBDIRS){
        exists($${app}/$${app}_App.pro) {
            $${app}_app.SUBDIRS = $${app}
            $${app}_app.file = $${app}/$${app}_App.pro
            APPLICATIONS += $${app}_app
        }
    }
    SUBDIRS += $$APPLICATIONS
}
