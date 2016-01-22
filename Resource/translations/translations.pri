# For autocompiling qm-files.

TRANSLATIONS = $$PWD/app_zh_CN.ts \
    $$PWD/app_zh_TW.ts \
    $$PWD/app_cs.ts \
    $$PWD/app_da.ts \
    $$PWD/app_de.ts \
    $$PWD/app_fr.ts \
    $$PWD/app_hu.ts \
    $$PWD/app_ja.ts \
    $$PWD/app_ko.ts \
    $$PWD/app_pl.ts \
    $$PWD/app_ru.ts \
    $$PWD/app_sk.ts \
    $$PWD/app_sl.ts \
    $$PWD/app_uk.ts

for(file, TRANSLATIONS) {
    TRANSLATIONS_TS_FILES += $${file}
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

#复制翻译文件到编译目录  
TRANSLATIONS_OUTPUT_PATH = $${TARGET_PATH}/translations
mytranslations.target = mytranslations
QT_QM = $$[QT_INSTALL_TRANSLATIONS]/qt_*.qm
equals(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH){
    TRANSLATIONS_OUTPUT_PATH = $$replace(TRANSLATIONS_OUTPUT_PATH, /, \\)
    QT_QM = $$system_path(QT_QM)
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
    mytranslations_commands += && $${QMAKE_COPY} $$QT_QM $${TRANSLATIONS_OUTPUT_PATH}\.
    mytranslations.commands = $$mytranslations_commands 
}
else {
    #QT_QM = $$replace(QT_QM, \\, /)
    mkpath($${TRANSLATIONS_OUTPUT_PATH})
    mytranslations.commands = \
        $${QMAKE_COPY} $${TRANSLATIONS_QM_FILES} $${TRANSLATIONS_OUTPUT_PATH}/. && \
        $${QMAKE_COPY_DIR} $$QT_QM $${TRANSLATIONS_OUTPUT_PATH}/.
}
!android{  #手机平台不需要  
    QMAKE_EXTRA_TARGETS += mytranslations
    #TODO:需要调试编译前编译翻译   
    #PRE_TARGETDEPS += mytranslations
    #发行版本才更新更新配置  
    TRANSLATIONS_FILE = $${TRANSLATIONS_OUTPUT_PATH}/app_zh_CN.qm
    equals(QMAKE_HOST.os, Windows){
        TRANSLATIONS_FILE = $$replace(TRANSLATIONS_FILE, /, \\)
    }
    !exists($${TRANSLATIONS_FILE}){
       POST_TARGETDEPS += mytranslations
    }
}

#安装资源文件  
mytranslat.files = $$TRANSLATIONS_QM_FILES $$QT_QM
mytranslat.path = $$PREFIX/translations
wince |android {
    DEPLOYMENT += mytranslat
}else{
    mytranslat.CONFIG += directory no_check_exist
    INSTALLS += mytranslat
}
