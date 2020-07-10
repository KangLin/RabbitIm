isEmpty(BUILD_VERSION) {
    isEmpty(GIT) : GIT=$$(GIT)
    isEmpty(GIT) : GIT=git
    isEmpty(GIT_DESCRIBE) {
        GIT_DESCRIBE = $$system(cd $$system_path($$_PRO_FILE_PWD_) && $$GIT describe --tags)
        isEmpty(BUILD_VERSION) {
            BUILD_VERSION = $$GIT_DESCRIBE
        }
    }
    isEmpty(BUILD_VERSION) {
        BUILD_VERSION = $$system(cd $$system_path($$_PRO_FILE_PWD_) && $$GIT rev-parse --short HEAD)
    }
    
    isEmpty(BUILD_VERSION){
        warning("Built without git, please add BUILD_VERSION to DEFINES or add git path to environment variable GIT or qmake parameter GIT")
    }
}
isEmpty(BUILD_VERSION): BUILD_VERSION="v0.1.0-382-ga1b87cf"
message("BUILD_VERSION:$$BUILD_VERSION")
DEFINES *= BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"
DEFINES *= RABBITIM_SYSTEM=\"\\\"$$quote($$RABBITIM_SYSTEM)\\\"\"
DEFINES *= RABBITIM_PLATFORM=\"\\\"$$quote($$RABBITIM_PLATFORM)\\\"\"
DEFINES *= RABBITIM_ARCHITECTURE=\"\\\"$$quote($$RABBITIM_ARCHITECTURE)\\\"\"

!win32{
    VERSION=$$replace(BUILD_VERSION, v,)
    VERSION=$$split(VERSION, -)
    VERSION=$$first(VERSION)
}

#doxygen必须位于的环境变量PATH中  
equals(RABBITIM_USE_DOXYGEN, 1):!android {
    #更新Doxyfile中的版本信息  
    DOXYFILE_FILENAME=$$OUT_PWD/Doxyfile
    TEMPLATE_DOXYFILE_FILENAME=$$PWD/../Doxyfile.template
    RABBITIM_DOXYFILE_CONTENTS=$$cat($$TEMPLATE_DOXYFILE_FILENAME, blob)
    RABBITIM_DOXYFILE_CONTENTS=$$replace(RABBITIM_DOXYFILE_CONTENTS, "%BUILD_VERSION%", $$BUILD_VERSION)
    RABBITIM_DOXYFILE_CONTENTS=$$replace(RABBITIM_DOXYFILE_CONTENTS, "%INPUT%", $$PWD)
    RABBITIM_DOXYFILE_CONTENTS=$$replace(RABBITIM_DOXYFILE_CONTENTS, "%OUTPUT_DIRECTORY%", $$OUT_PWD/Doxygen)
    #message($$RABBITIM_DOXYFILE_CONTENTS)
    write_file($$DOXYFILE_FILENAME, RABBITIM_DOXYFILE_CONTENTS)
    message("Generate doxygen documents ......")
    DOXYGEN_RETURN=$$system(doxygen $$DOXYFILE_FILENAME)
    #message("DOXYGEN_RETURN:$$DOXYGEN_RETURN")
}
