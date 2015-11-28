mkpath($$_PRO_FILE_PWD_/translations) #插件需要建立此目录  

# For autocompiling qm-files.
TRANSLATIONS = translations/Plugin_zh_CN.ts \
    translations/Plugin_en.ts

for(file, TRANSLATIONS) {
    TRANSLATIONS_TS_FILES += $$_PRO_FILE_PWD_/$${file}
}
TRANSLATIONS_QM_FILES = $$replace(TRANSLATIONS_TS_FILES, ".ts", ".qm")

#rules to generate ts
isEmpty(QMAKE_LUPDATE) {
    win32: QMAKE_LUPDATE = $$[QT_INSTALL_BINS]/lupdate.exe
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
    win32: QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease.exe
    else:  QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.name = Compiling qm-files...
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link  no_clean target_predeps 
QMAKE_EXTRA_COMPILERS += updateqm

TRANSLATIONS_OUTPUT_PATH = $${DESTDIR}/translations
mytranslations.target = mytranslations
equals(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH){
    TRANSLATIONS_OUTPUT_PATH = $$replace(TRANSLATIONS_OUTPUT_PATH, /, \\)
    mkpath($${TRANSLATIONS_OUTPUT_PATH})
    TRANSLATIONS_QM_FILES = $$replace(TRANSLATIONS_QM_FILES, /, \\)
    for(file, TRANSLATIONS_QM_FILES){
        isEmpty(mytranslations_commands){
            mytranslations_commands += $${QMAKE_COPY} $${file} \
                                    $${TRANSLATIONS_OUTPUT_PATH}\. 
        }
        else {
            mytranslations_commands += && $${QMAKE_COPY} $${file} \
                                    $${TRANSLATIONS_OUTPUT_PATH}\. 
        }
    }
    mytranslations.commands = $$mytranslations_commands 
}
else {
    mkpath($${TRANSLATIONS_OUTPUT_PATH})
    mytranslations.commands = \
        $${QMAKE_COPY} $${TRANSLATIONS_QM_FILES} $${TRANSLATIONS_OUTPUT_PATH}/. 
}
!android{  #手机平台不需要  
    QMAKE_EXTRA_TARGETS += mytranslations
    #TODO:需要调试编译前编译翻译   
    #POST_TARGETDEPS += mytranslations
    #发行版本才更新更新配置  
    CONFIG(release, debug|release) {
        POST_TARGETDEPS += mytranslations
    }
}

mytranslat.files = $$TRANSLATIONS_QM_FILES
mytranslat.path = $$PREFIX/plugins/App/$${TARGET}/translations

wince |android {
    DEPLOYMENT += mytranslat
}else{
    mytranslat.CONFIG += directory no_check_exist 
    INSTALLS += mytranslat
}
