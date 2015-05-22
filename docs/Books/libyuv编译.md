libyuv编译
=========

## 下载代码(要翻墙)：

    git clone http://git.chromium.org/external/libyuv.git    

## 编译：

1. 用 make 编译（linux、mingw、cygwin）：

    make -f linux.mk

2. 用 cmake 编译（vs、linux、mingw、msys等）:

    mkdir out
    cd out
    cmake ..
    cmake --build .
    
    Release build/install
     mkdir out
     cd out
     cmake -DCMAKE_INSTALL_PREFIX="/usr/lib" -DCMAKE_BUILD_TYPE="Release" ..
     cmake --build . --config Release
     sudo cmake --build . --target install --config Release

3. Windows 8 Phone

* 编译环境准备：  
 安装 Visual Studio 2012 and Arm 到你的环境：


   call "c:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_arm\vcvarsx86_arm.bat"

 
或者 Visual Studio 2013


   call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_arm\vcvarsx86_arm.bat"


也可以从“开始”菜单->Visual Studio 2013->Visual Studio Tools->VS2013 ARM 兼容工具命令提示

* 然后再进入libyuv源码根目录，再执行下列操作：


    nmake /f winarm.mk clean
    nmake /f winarm.mk

 
4. android:


    #需要设置下面变量：
    export ANDROID_NDK_ROOT=/home/android-ndk-r9c               #指定 android ndk 根目录 
    export ANDROID_NDK=$ANDROID_NDK_ROOT  #指定 android ndk 根目录 
    export ANDROID_SDK=/home/android-sdk/sdk                   #指定 android sdk 根目录
    export ANDROID_SDK_ROOT=$ANDROID_SDK   
    export JAVA_HOME=/home/jdk1.7.0_51                      #指定 jdk 根目录 
    
    cmake .. \
        -G"Unix Makefiles"\
        -DCMAKE_MAKE_PROGRAM="$ANDROID_NDK/prebuilt/${HOST}/bin/make" \ #android 自带的 make
        -DCMAKE_INSTALL_PREFIX="$PREFIX" \
        -DCMAKE_TOOLCHAIN_FILE="${RabbitImRoot}/platforms/android/android.toolchain.cmake"
    
    echo "build..."
    cmake --build . --target install --config Release


Getting Started

How to get and build the libyuv code. 

Pre-requisite

You'll need to have depot tools installed: 

https://sites.google.com/a/chromium.org/dev/developers/how-tos/depottools 

Refer to chromium instructions for each platform for other prerequesites:


Getting the Code

Create a working directory, enter it, and run: 


 gclient config http://libyuv.googlecode.com/svn/trunk 
 gclient sync
 
 

 solutions = [
   { "name"        : "trunk",
     "url"         : "https://libyuv.googlecode.com/svn/trunk",
     "deps_file"   : "DEPS",
     "managed"     : True,
     "custom_deps" : {
     },
     "safesync_url": "",
   },
 ];
 


For iOS add ;target_os=['ios']; to your OSX .gclient and run gclient sync --nohooks twice.
 For Android add ;target_os=['android']; to your Linux .gclient and run gclient sync --nohooks


The sync will generate native build files for your environment using gyp (Windows: Visual Studio, OSX: XCode, Linux: make). This generation can also be forced manually: gclient runhooks 

Git

A git mirror is available here http://git.chromium.org/gitweb/?p=external/libyuv.git;a=summary 

Building the Library and Unittests

Note: To build just the library and not the tests, change libyuv_test.gyp to libyuv.gyp 

Windows

 set GYP_DEFINES=target_arch=ia32 
 call python gyp_libyuv -fninja -G msvs_version=2013 --depth=. libyuv_test.gyp 
 ninja -j7 -C out\Release 
 ninja -j7 -C out\Debug 
 

set GYP_DEFINES=target_arch=x64
 call python gyp_libyuv -fninja -G msvs_version=2013 --depth=. libyuv_test.gyp
 ninja -C out\Debug_x64
 ninja -C out\Release_x64
 




OSX

# Clang 64 bit shown. Remove clang=1 for gcc and change x64 to ia32 for 32 bit 

 GYP_DEFINES="clang=1 target_arch=x64" ./gyp_libyuv -fninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug 
 ninja -j7 -C out/Release 
 

GYP_DEFINES="clang=1 target_arch=ia32" ./gyp_libyuv -fninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug 
 ninja -j7 -C out/Release 
 




Linux

 tools/clang/scripts/update.sh
 GYP_DEFINES="target_arch=x64" ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug 
 ninja -j7 -C out/Release 
 

GYP_DEFINES="target_arch=ia32" ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug 
 ninja -j7 -C out/Release 
 




iOS

http://www.chromium.org/developers/how-tos/build-instructions-ios
 Add to .gclient last line: target_os=['ios']; 

armv7 
 GYP_DEFINES="OS=ios target_arch=armv7" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_ios" ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out_ios/Debug-iphoneos libyuv_unittest
 ninja -j7 -C out_ios/Release-iphoneos libyuv_unittest
 


arm64 
 GYP_DEFINES="OS=ios target_arch=armv7 target_subarch=64" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_ios" ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out_ios/Debug-iphoneos libyuv_unittest
 ninja -j7 -C out_ios/Release-iphoneos libyuv_unittest
 


Android

https://code.google.com/p/chromium/wiki/AndroidBuildInstructions
 Add to .gclient last line: target_os=['android']; 

armv7 
 GYP_DEFINES="OS=android" GYP_CROSSCOMPILE=1 ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug libyuv_unittest
 ninja -j7 -C out/Release libyuv_unittest
 


arm64 
 GYP_DEFINES="OS=android target_arch=arm64" GYP_CROSSCOMPILE=1 ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug libyuv_unittest
 ninja -j7 -C out/Release libyuv_unittest
 


ia32 
 GYP_DEFINES="OS=android target_arch=ia32" GYP_CROSSCOMPILE=1 ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug libyuv_unittest
 ninja -j7 -C out/Release libyuv_unittest
 


mipsel 
 GYP_DEFINES="OS=android target_arch=mipsel" GYP_CROSSCOMPILE=1 ./gyp_libyuv -f ninja --depth=. libyuv_test.gyp 
 ninja -j7 -C out/Debug libyuv_unittest
 ninja -j7 -C out/Release libyuv_unittest
 


arm64 disassembly: 
 ./third_party/android_tools/ndk//toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-objdump -d out/Release/obj/source/libyuv_neon.row_neon64.o
 


Old method: 
 source build/android/envsetup.sh
 gclient runhooks
 ninja -j7 -C out/Debug 
 ninja -j7 -C out/Release 
 


Windows Shared Library

Modify libyuv.gyp from 'static_library' to 'shared_library', and add 'LIBYUV_BUILDING_SHARED_LIBRARY' to 'defines'. 
 gclient runhooks 
 After this command follow the building the library instructions above.


If you get a compile error for atlthunk.lib on Windows, read http://www.chromium.org/developers/how-tos/build-instructions-windows


Build targets

 ninja -C out/Debug libyuv
 ninja -C out/Debug libyuv_unittest
 ninja -C out/Debug compare
 ninja -C out/Debug convert
 ninja -C out/Debug psnr
 ninja -C out/Debug cpuid
 


Building the Library with make

Linux

 make -j7 V=1 -f linux.mk
 make -j7 V=1 -f linux.mk clean
 make -j7 V=1 -f linux.mk CXX=clang++
 


Building the Library with cmake

Install cmake: http://www.cmake.org/ 
 Default debug build:
 mkdir out
 cd out
 cmake ..
 cmake --build . 
 

Release build/install
 mkdir out
 cd out
 cmake -DCMAKE_INSTALL_PREFIX="/usr/lib" -DCMAKE_BUILD_TYPE="Release" ..
 cmake --build . --config Release
 sudo cmake --build . --target install --config Release
 






Windows 8 Phone

Pre-requisite:
 Install Visual Studio 2012 and Arm to your environment.

 call "c:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_arm\vcvarsx86_arm.bat"
 
or Visual Studio 2013
 call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_arm\vcvarsx86_arm.bat"
 nmake /f winarm.mk clean
 nmake /f winarm.mk
 


Building the Library with native builds (deprecated)

Windows

 gclient runhooks 
 devenv -build "Debug" libyuv.sln 
 devenv -build "Release" libyuv.sln 
 


OSX

 gclient runhooks 
 xcodebuild -project libyuv.xcodeproj -configuration Debug 
 xcodebuild -project libyuv.xcodeproj -configuration Release 
 


Linux

 gclient runhooks 
 make -j7 V=1 -r libyuv BUILDTYPE=Debug 
 make -j7 V=1 -r libyuv BUILDTYPE=Release 
 


iOS

 GYP_DEFINES="OS=ios target_arch=armv7 armv7=1 arm_neon=1" GYP_CROSSCOMPILE=1 ./gyp_libyuv  --depth=. libyuv_test.gyp 
 xcodebuild -project libyuv.xcodeproj -configuration Debug -arch armv7 -sdk iphoneos7.0 
 xcodebuild -project libyuv.xcodeproj -configuration Release -arch armv7 -sdk iphoneos7.0 
 


Windows Shared Library

Modify libyuv.gyp from 'static_library' to 'shared_library', and add 'LIBYUV_BUILDING_SHARED_LIBRARY' to 'defines'. Then run this.
 gclient runhooks 
 After this command follow the building the library instructions above.


If you get a compile error for atlthunk.lib on Windows, read http://www.chromium.org/developers/how-tos/build-instructions-windows 

64 bit Windows

 set GYP_DEFINES=target_arch=x64 
 gclient runhooks V=1 
 


ARM Linux

 $ export GYP_DEFINES="target_arch=arm" 
 $ export CROSSTOOL=<path>/arm-none-linux-gnueabi 
 $ export CXX=$CROSSTOOL-g++ 
 $ export CC=$CROSSTOOL-gcc 
 $ export AR=$CROSSTOOL-ar 
 $ export AS=$CROSSTOOL-as 
 $ export RANLIB=$CROSSTOOL-ranlib 
 $ gclient runhooks 
 


Running Unittests

Windows

 out\Release\libyuv_unittest.exe --gtest_catch_exceptions=0 --gtest_filter=* 
 


OSX

 out/Release/libyuv_unittest --gtest_filter=* 
 


Linux

 out/Release/libyuv_unittest --gtest_filter=* 
 


Replace --gtest_filter=* with specific unittest to run. May include wildcards. e.g.


 out/Release/libyuv_unittest --gtest_filter=libyuvTest.I420ToARGB_Opt
 


CPU Emulator tools

Intel SDE (Software Development Emulator)

Pre-requisite:
 Install IntelSDE for Windows.
http://software.intel.com/en-us/articles/intel-software-development-emulator 

 c:\intelsde\sde -hsw -- out\release\libyuv_unittest.exe --gtest_filter=* 


Memory tools

Running Dr Memory memcheck for Windows

Pre-requisite:
 Install Dr Memory for Windows and add it to your path.
http://www.drmemory.org/docs/page_install_windows.html 

 set GYP_DEFINES=build_for_tool=drmemory target_arch=ia32 
 call python gyp_libyuv -fninja -G msvs_version=2012 --depth=. libyuv_test.gyp 
 ninja -C out\Debug 
 drmemory out\Debug\libyuv_unittest.exe --gtest_catch_exceptions=0 --gtest_filter=* 
 


Running Valgrind memcheck

Memory errors and race conditions can be found by running tests under special memory tools. Valgrind is an instrumentation framework for building dynamic analysis tools. Various tests and profilers are built upon it to find memory handling errors and memory leaks, for instance. 

 solutions = [
   { "name"        : "trunk",
     "url"         : "http://libyuv.googlecode.com/svn/trunk",
     "deps_file"   : "DEPS",
     "managed"     : True,
     "custom_deps" : {
        "third_party/valgrind": "http://src.chromium.org/svn/trunk/deps/third_party/valgrind/binaries",
     },
     "safesync_url": "",
   },
 ]
 

GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=memcheck" python gyp_libyuv -fninja --depth=. libyuv_test.gyp
 ninja -C out/Debug
 valgrind out/Debug/libyuv_unittest
 




For more information, see http://www.chromium.org/developers/how-tos/using-valgrind 

Running Thread Sanitizer (tsan)

 GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=tsan" python gyp_libyuv -fninja --depth=. libyuv_test.gyp
 ninja -C out/Debug
 valgrind out/Debug/libyuv_unittest
 


For more info, see http://www.chromium.org/developers/how-tos/using-valgrind/threadsanitizer 

Running Address Sanitizer (asan)

 GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=asan" python gyp_libyuv -fninja --depth=. libyuv_test.gyp
 ninja -C out/Debug
 valgrind out/Debug/libyuv_unittest
 


For more info, see http://dev.chromium.org/developers/testing/addresssanitizer 

Benchmarking

The unittests can be used to benchmark.



Windows

 set LIBYUV_WIDTH=1280
 set LIBYUV_HEIGHT=720
 set LIBYUV_REPEAT=1000
 out\Release\libyuv_unittest.exe --gtest_filter=*I420ToARGB_Opt 
 


Linux and Mac

 LIBYUV_WIDTH=1280 LIBYUV_HEIGHT=720 LIBYUV_REPEAT=1000 out/Release/libyuv_unittest --gtest_filter=*I420ToARGB_Opt 
   
 libyuvTest.I420ToARGB_Opt (547 ms) 
 
Indicates 0.547 ms/frame for 1280 x 720. 
