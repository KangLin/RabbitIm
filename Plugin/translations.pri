mkpath($$_PRO_FILE_PWD_/translations) #插件需要建立此目录  

# For autocompiling qm-files.
TRANSLATIONS = translations/Plugin_zh_CN.ts \
    translations/Plugin_zh_TW.ts \
    translations/Plugin_cs.ts \
    translations/Plugin_da.ts \
    translations/Plugin_de.ts \
    translations/Plugin_fr.ts \
    translations/Plugin_hu.ts \
    translations/Plugin_ja.ts \
    translations/Plugin_ko.ts \
    translations/Plugin_pl.ts \
    translations/Plugin_ru.ts \
    translations/Plugin_sk.ts \
    translations/Plugin_sl.ts \
    translations/Plugin_uk.ts

for(file, TRANSLATIONS) {
    TRANSLATIONS_TS_FILES += $$_PRO_FILE_PWD_/$${file}
}
TRANSLATIONS_QM_FILES = $$replace(TRANSLATIONS_TS_FILES, "\.ts", ".qm")

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
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link  no_clean target_predeps 
QMAKE_EXTRA_COMPILERS += updateqm

#静态库或android下生成翻译资源文件  
android | CONFIG(static, static|shared) {
    TRANSLATIONS_RESOURCES_FILE = $$_PRO_FILE_PWD_/translations/Translations.qrc
    #生成资源文件  
    TRANSLATIONS_RESOURCES_CONTENT = "</qresource></RCC>"
    FILE_CONTENT = $$cat($$TRANSLATIONS_RESOURCES_FILE) 
    !contains(FILE_CONTENT, $$TRANSLATIONS_RESOURCES_CONTENT){
        TRANSLATIONS_RESOURCES_CONTENT = "<RCC><qresource prefix='/translations'>"
        for(file, TRANSLATIONS_QM_FILES) {
            TRANSLATIONS_RESOURCES_CONTENT += "<file>$$replace(file, "$$_PRO_FILE_PWD_/translations/", "")</file>"
        }
        TRANSLATIONS_RESOURCES_CONTENT += "</qresource></RCC>"
        write_file($$TRANSLATIONS_RESOURCES_FILE, TRANSLATIONS_RESOURCES_CONTENT, append)
    }
    #包含资源文件  
    RESOURCES += $$TRANSLATIONS_RESOURCES_FILE
}

#复制翻译文件到编译目录  
TRANSLATIONS_OUTPUT_PATH = $${TARGET_PATH}/translations
mytranslations.target = mytranslations
equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
        TRANSLATIONS_OUTPUT_PATH = $$system_path($${TRANSLATIONS_OUTPUT_PATH})
}
mkpath($${TRANSLATIONS_OUTPUT_PATH})
for(file, TRANSLATIONS_QM_FILES){
    equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
        file = $$system_path($${file})
    }
    isEmpty(mytranslations_commands){
        mytranslations_commands += $${QMAKE_COPY} "$${file}" \
                               "$${TRANSLATIONS_OUTPUT_PATH}"
    }
    else {
        mytranslations_commands += && $${QMAKE_COPY} "$${file}" \
                                "$${TRANSLATIONS_OUTPUT_PATH}" 
    }
}
mytranslations.commands = $$mytranslations_commands 

!android{  #手机平台不需要  
    QMAKE_EXTRA_TARGETS += mytranslations
    #TODO:需要调试编译前编译翻译   
    #POST_TARGETDEPS += mytranslations
    #发行版本才更新更新配置  
    #CONFIG(release, debug|release) {
    TRANSLATIONS_FILE = $${TRANSLATIONS_OUTPUT_PATH}/Plugin_zh_CN.qm
    equals(QMAKE_HOST.os, Windows){
        TRANSLATIONS_FILE = $$replace(TRANSLATIONS_FILE, /, \\)
    }
    !exists($${TRANSLATIONS_FILE}){
        POST_TARGETDEPS += mytranslations
    }
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
