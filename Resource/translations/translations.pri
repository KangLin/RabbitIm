# For autocompiling qm-files.

TRANSLATIONS = Resource/translations/app_zh_CN.ts 

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

# Release all the .ts files at once
#updateallqm = $$QMAKE_LRELEASE -silent $$TRANSLATIONS

TRANSLATIONS_OUTPUT_PATH = $${TARGET_PATH}/translations
mytranslations.target = mytranslations
msvc {
    TRANSLATIONS_OUTPUT_PATH = $$replace(TRANSLATIONS_OUTPUT_PATH, /, \\)
    QT_QM = $$[QT_INSTALL_TRANSLATIONS]\qt_zh_CN.qm
    QT_QM = $$replace(QT_QM, /, \\)
    mkpath($${TRANSLATIONS_OUTPUT_PATH})
    mytranslations.commands =  \
        $(COPY) $$replace(PWD, /, \\)\app_zh_CN.qm $${TRANSLATIONS_OUTPUT_PATH}\app_zh_CN.qm && \
        $(COPY) $$QT_QM $${TRANSLATIONS_OUTPUT_PATH}\qt_zh_CN.qm
    #mytranslations.depends = mytranslations_path
}
else {
    mytranslations.commands = $(MKDIR) $${TRANSLATIONS_OUTPUT_PATH} && \
        $(COPY_DIR) $${PWD}/app_zh_CN.qm $${TRANSLATIONS_OUTPUT_PATH}/app_zh_CN.qm && \
        $(COPY_DIR) $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm $${TRANSLATIONS_OUTPUT_PATH}/qt_zh_CN.qm
}
QMAKE_EXTRA_TARGETS += mytranslations
#PRE_TARGETDEPS += mytranslations
POST_TARGETDEPS += mytranslations

#安装
isEmpty(PREFIX)
{
    android{
       PREFIX=/.
    }
    else{
        PREFIX = install
    }
}
wince {
    mytranslat.files = Resource/translations/app_zh_CN.qm
    mytranslat.path = $$PREFIX
    DEPLOYMENT += mytranslat
}
else{
    other.files = License.html Authors.txt ChangeLog.txt README.md
    other.path = $$PREFIX
    other.CONFIG += directory no_check_exist 
    target.path = $$PREFIX
    mytranslat.files =  Resource/translations/app_zh_CN.qm $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm
    mytranslat.path = $$PREFIX/translate
    mytranslat.CONFIG += directory no_check_exist 
    INSTALLS += target other mytranslat
}
