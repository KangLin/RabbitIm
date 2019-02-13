# For autocompiling qm-files.
LOCALE_LIST = zh_CN \
    zh_TW \
    cs \
    da \
    de \
    fr \
    hu \
    ja \
    ko \
    pl \
    ru \
    sk \
    sl \
    uk

TRANSLATIONS_OUTPUT_PATH = $${TARGET_PATH}/translations
equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
        TRANSLATIONS_OUTPUT_PATH = $$system_path($${TRANSLATIONS_OUTPUT_PATH})
}
mkpath($${TRANSLATIONS_OUTPUT_PATH})

for(v, LOCALE_LIST){
    TRANSLATIONS += $$PWD/app_$${v}.ts
    TRANSLATIONS_QM_FILES += $${TRANSLATIONS_OUTPUT_PATH}/app_$${v}.qm
}

#rules to generate ts
isEmpty(QMAKE_LUPDATE) {
    win32: QMAKE_LUPDATE = $$system_path($$[QT_INSTALL_BINS]/lupdate.exe)
    else:  QMAKE_LUPDATE = $$[QT_INSTALL_BINS]/lupdate
}

#limitation: only on ts can be generated
updatets.name = Creating or updating ts-files...
updatets.input = _PRO_FILE_
updatets.output = $$TRANSLATIONS
updatets.commands = $$QMAKE_LUPDATE ${QMAKE_FILE_IN}
updatets.CONFIG += no_link no_clean
QMAKE_EXTRA_COMPILERS += updatets

#rules for ts->qm
isEmpty(QMAKE_LRELEASE) {
    win32: QMAKE_LRELEASE = $$system_path($$[QT_INSTALL_BINS]/lrelease.exe)
    else:  QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.name = Compiling qm-files...
updateqm.input = TRANSLATIONS
updateqm.output = $${TRANSLATIONS_OUTPUT_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link  no_clean target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

#静态库或android下生成翻译资源文件  
android | CONFIG(static, static|shared) {
    TRANSLATIONS_RESOURCES_FILE = $$TRANSLATIONS_OUTPUT_PATH/Translations.qrc
    #生成资源文件  
    TRANSLATIONS_RESOURCES_CONTENT = "</qresource></RCC>"
    FILE_CONTENT = $$cat($$TRANSLATIONS_RESOURCES_FILE) 
    !contains(FILE_CONTENT, $$TRANSLATIONS_RESOURCES_CONTENT){
        TRANSLATIONS_RESOURCES_CONTENT = "<RCC><qresource prefix='/translations'>"
        for(file, TRANSLATIONS_QM_FILES) {
            TRANSLATIONS_RESOURCES_CONTENT += "<file>$$replace(file, "$$TRANSLATIONS_OUTPUT_PATH/", "")</file>"
        }
        TRANSLATIONS_RESOURCES_CONTENT += "</qresource></RCC>"
        write_file($$TRANSLATIONS_RESOURCES_FILE, TRANSLATIONS_RESOURCES_CONTENT, append)
    }
    #包含资源文件  
    RESOURCES += $$TRANSLATIONS_RESOURCES_FILE
}

#复制QT的翻译资源
QT_QM = $$[QT_INSTALL_TRANSLATIONS]/qt_*.qm
equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
    QT_QM = $$system_path($${QT_QM})
}
#mytranslations.target = mytranslations
#mytranslations_commands = $${QMAKE_COPY} "$${QT_QM}" "$${TRANSLATIONS_OUTPUT_PATH}"
#mytranslations.commands = $$mytranslations_commands 

#!android{  #手机平台不需要  
#    QMAKE_EXTRA_TARGETS += mytranslations
#    #TODO:需要调试编译前编译翻译   
#    #PRE_TARGETDEPS += mytranslations
#    #发行版本才更新更新配置  
#    
#    POST_TARGETDEPS += mytranslations
#}

#安装资源文件  
mytranslat.files = $$TRANSLATIONS_QM_FILES $$QT_QM
mytranslat.path = $$PREFIX/translations

wince |android {
    DEPLOYMENT += mytranslat
}else{
    mytranslat.CONFIG += no_check_exist
    INSTALLS += mytranslat
}
