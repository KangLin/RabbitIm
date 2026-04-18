Package: @CPACK_DEBIAN_PACKAGE_NAME@
Architecture: any
Version: @CPACK_DEBIAN_PACKAGE_VERSION@
Section: x11
Priority: @CPACK_DEBIAN_PACKAGE_PRIORITY@
Depends:  ${shlibs:Depends}, ${misc:Depends}, libavcodec54 (>= 6:9.1-1) | libavcodec-extra-54 (>= 6:9.16), libavutil52 (>= 6:9.1-1), libc6 (>= 2.14), libcurl3 (>= 7.16.2), libegl1-mesa (>= 7.8.1) | libegl1-x11, libfontconfig1 (>= 2.9.0), libfreetype6 (>= 2.3.5), libgcc1 (>= 1:4.1.1), libgl1-mesa-glx | libgl1, libglib2.0-0 (>= 2.22.0), libgtk2.0-0 (>= 2.24.0), libice6 (>= 1:1.0.0), libicu52 (>= 52~m1-1~), libpango-1.0-0 (>= 1.14.0), libpulse0 (>= 1:0.99.1), libqt5core5a (>= 5.2.0), libqt5dbus5 (>= 5.0.2), libqt5gui5 (>= 5.2.0) | libqt5gui5-gles (>= 5.2.0), libqt5multimedia5 (>= 5.0.2), libqt5network5 (>= 5.0.2), libqt5opengl5 (>= 5.0.2) | libqt5opengl5-gles (>= 5.0.2), libqt5printsupport5 (>= 5.0.2), libqt5sensors5 (>= 5.1.1), libqt5webkit5, libqt5widgets5 (>= 5.2.0), libqt5xml5 (>= 5.0.2), libsm6, libssl1.0.0 (>= 1.0.0), libstdc++6 (>= 4.6), libswscale2 (>= 6:9.1-1), libvpx1 (>= 1.0.0), libx11-6, libx11-xcb1, libxcb1, libxi6 (>= 2:1.2.99.4), libxrender1, qtbase-abi-5-2-1
Maintainer: @CPACK_DEBIAN_PACKAGE_MAINTAINER@
Description: @CPACK_DEBIAN_PACKAGE_DESCRIPTION@
