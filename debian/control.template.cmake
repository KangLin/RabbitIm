Source: @CPACK_DEBIAN_PACKAGE_NAME@
Section: x11
Priority: @CPACK_DEBIAN_PACKAGE_PRIORITY@
Maintainer: @CPACK_DEBIAN_PACKAGE_MAINTAINER@
Build-Depends: debhelper (>= 8.0.0), cmake, @CPACK_DEBIAN_PACKAGE_DEPENDS@
Standards-Version: @CPACK_DEBIAN_PACKAGE_VERSION@
Homepage: https://github.com/KangLin/rabbitim
Vcs-Git: git@github.com:KangLin/rabbitim.git
Vcs-Browser: https://github.com/KangLin/rabbitim

Package: @CPACK_DEBIAN_PACKAGE_NAME@
Architecture: any
Version: @CPACK_DEBIAN_PACKAGE_VERSION@
Section: x11
Priority: @CPACK_DEBIAN_PACKAGE_PRIORITY@
Depends:  ${shlibs:Depends}, ${misc:Depends}
Maintainer: @CPACK_DEBIAN_PACKAGE_MAINTAINER@
Description: @CPACK_DEBIAN_PACKAGE_DESCRIPTION@
