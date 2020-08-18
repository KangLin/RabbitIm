#RABBITIM_USE_PLUGIN_APP=1 #生成插件应用程序  

TEMPLATE = subdirs

greaterThan(QT_VERSION, 5.4) {
    SUBDIRS += App/Lbs
}

SUBDIRS += ConverFormat/FFmpeg #ConverFormat/OpenCV
#SUBDIRS += Protocol/ProtocolQXmpp

SUBDIRS += App/QRCode \
    App/ScreenShot

#CONFIG(debug, debug|release) {
    #TODO:增加需要调试的插件  
#    SUBDIRS = ProtocolQXmpp
#    message("Debug plugin:$${SUBDIRS}")
#}
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

OTHER_FILES += CMakeLists.txt \
               pri/*
