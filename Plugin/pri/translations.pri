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

TRANSLATIONS_OUTPUT_PATH = $$DESTDIR/translations
equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
        TRANSLATIONS_OUTPUT_PATH = $$system_path($${TRANSLATIONS_OUTPUT_PATH})
}
mkpath($${TRANSLATIONS_OUTPUT_PATH})

for(v, LOCALE_LIST){
    TRANSLATIONS += $$_PRO_FILE_PWD_/translations/Plugin_$${v}.ts
    TRANSLATIONS_QM_FILES += $${TRANSLATIONS_OUTPUT_PATH}/Plugin_$${v}.qm
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
mkpath($$_PRO_FILE_PWD_/translations) #插件需要建立此目录  
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

#安装资源文件  
mytranslat.files = $$TRANSLATIONS_QM_FILES
#TODO:修改App路径  
mytranslat.path = $${TARGET_INSTALL_PATH}/translations

wince |android {
    DEPLOYMENT += mytranslat
}else : !CONFIG(static, static|shared){
    mytranslat.CONFIG += no_check_exist 
    INSTALLS += mytranslat
}
