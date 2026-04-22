# See: https://rpm-software-management.github.io/rpm/manual/spec.html

%define   INSTALL_PREFIX /opt/RabbitIm

Name:           rabbitim
Version:        0.1.0_dev
Release:        1%{?dist}
Summary:        玉兔即时通讯是一款跨操作系统平台、支持多种协议的即时通讯软件

License:        GPL-3.0
URL:            https://github.com/KangLin/RabbitIm
Source0:        RabbitIm.tar.gz

Vendor:         Kang Lin <kl222@126.com>

BuildRequires: make git rpm-build rpmdevtools 
BuildRequires: gcc-c++
BuildRequires: automake autoconf libtool gettext gettext-autopoint
BuildRequires: cmake desktop-file-utils appstream curl wget
#BuildRequires: appstream

# Qt6
BuildRequires: qt6-qttools-devel qt6-qtbase-devel
BuildRequires: qt6-qt5compat-devel qt6-qtmultimedia-devel qt6-qtscxml-devel
BuildRequires: qt6-qtsvg-devel qt6-qtspeech-devel
BuildRequires: qt6-qtpositioning-devel

BuildRequires: qxmpp-qt6-devel

Requires: openssl libpng libjpeg-turbo pixman openh264 ffmpeg-free libssh libicu lzo libgcrypt
Requires: libvncserver libwinpr freerdp fuse3 libpcap libcurl
Requires: zlib gnutls nettle gmp pam 
Requires: qt6-qtbase qt6-qtbase-mysql qt6-qtbase-odbc qt6-qtbase-postgresql
Requires: qt6-qtmultimedia qt6-qt5compat qt6-qtscxml qt6-qtsvg
Requires: qt6-qtpositioning qt6-qtspeech
Requires: qxmpp-qt6 zxing-cpp
Requires: mysql-libs

%description
玉兔即时通讯是一款跨操作系统平台、支持多种协议的即时通讯软件

作者: Kang Lin <kl222@126.com>

捐赠:

https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png

https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png

https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png

# 预备参数
%prep
%setup -q -n RabbitIm

%generate_buildrequires

%build
echo "-- RPM_BUILD_DIR: $RPM_BUILD_DIR"
echo "-- RPM_BUILD_ROOT: $RPM_BUILD_ROOT"
echo "-- RPM_SOURCE_DIR: $RPM_SOURCE_DIR"
echo "-- RPM_SPECPARTS_DIR: $RPM_SPECPARTS_DIR"
echo "-- pwd: `pwd`"

cmake . -B $RPM_BUILD_DIR  \
    -DCMAKE_INSTALL_PREFIX=%{INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=Release \
    -DRabbitCommon_ROOT=${RabbitCommon_ROOT} \
    -DCMARK_SHARED=OFF \
    -DCMARK_TESTS=OFF \
    -DCMARK_STATIC=ON \
    -DWITH_CMARK=OFF \
    -DWITH_CMARK_GFM=ON \
    -DENABLE_UPDATE_TRANSLATIONS=ON \
    -DRABBIT_ENABLE_INSTALL_TARGETS=ON ${CMAKE_CONFIG_PARAS}
cmake --build $RPM_BUILD_DIR --config Release --parallel $(nproc)

%install
cmake --install $RPM_BUILD_DIR --config Release --strip \
    --component DependLibraries \
    --prefix ${RPM_BUILD_ROOT}%{INSTALL_PREFIX}
cmake --install $RPM_BUILD_DIR --config Release --strip \
    --component Runtime \
    --prefix ${RPM_BUILD_ROOT}%{INSTALL_PREFIX}
cmake --install $RPM_BUILD_DIR --config Release --strip \
    --component Plugin \
    --prefix ${RPM_BUILD_ROOT}%{INSTALL_PREFIX}
cmake --install $RPM_BUILD_DIR --config Release --strip \
    --component Application \
    --prefix ${RPM_BUILD_ROOT}%{INSTALL_PREFIX}

#if [ -n "${INSTALL_DIR}" ]; then
#    SOURCE_DIR=${INSTALL_DIR}%{_libdir}
#    DEST_DIR=${RPM_BUILD_ROOT}%{INSTALL_PREFIX}%{_libdir}
#    # 复制 .so 库文件及其符号链接
#    find $SOURCE_DIR -name "*.so*" -exec cp -d {} $DEST_DIR \;
#fi

# 安装前需要做的任务，如：创建用户
%pre

# 安装后需要做的任务 如：自动启动的任务
%post
INSTALL_PREFIX=/opt/RabbitIm
if [ ! -f /usr/share/applications/io.github.KangLin.RabbitIm.desktop ]; then
    if [ -f $INSTALL_PREFIX/share/applications/io.github.KangLin.RabbitIm.desktop ]; then
        ln -s $INSTALL_PREFIX/share/applications/io.github.KangLin.RabbitIm.desktop /usr/share/applications/io.github.KangLin.RabbitIm.desktop
        chmod a+rx /usr/share/applications/io.github.KangLin.RabbitIm.desktop
    fi
fi
if [ ! -f /usr/share/pixmaps/io.github.KangLin.RabbitIm.png ]; then
    if [ -f $INSTALL_PREFIX/share/pixmaps/io.github.KangLin.RabbitIm.png ]; then
        if [ ! -d /usr/share/pixmaps ]; then
            mkdir -p /usr/share/pixmaps
        fi
        ln -s $INSTALL_PREFIX/share/pixmaps/io.github.KangLin.RabbitIm.png /usr/share/pixmaps/io.github.KangLin.RabbitIm.png
    fi
fi
if [ ! -f /usr/bin/RabbitIm ]; then
    ln -s $INSTALL_PREFIX/bin/RabbitImApp /usr/bin/RabbitIm
fi

if [ -d $INSTALL_PREFIX/etc ]; then
    chmod -R a+rw $INSTALL_PREFIX/etc
fi

# 卸载前需要做的任务 如：停止任务
%preun
rm -fr /usr/share/applications/io.github.KangLin.RabbitIm.desktop
rm -fr /usr/share/pixmaps/io.github.KangLin.RabbitIm.png
rm -fr /usr/bin/RabbitIm

# 卸载后需要做的任务 如：删除用户，删除/备份业务数据
%postun

# 清除上次编译生成的临时文件
%clean

# 设置文件属性，包含编译文件需要生成的目录、文件以及分配所对应的权限
%files
%dir %{INSTALL_PREFIX}

%{INSTALL_PREFIX}/*

# 修改历史
%changelog
* %{build_time} Kang Lin <kl222@126.com> - %{version}
- Initial RPM package
