#!/bin/sh
set -ev

sudo add-apt-repository -y ppa:ubuntu-sdk-team/ppa
sudo apt-get update -y -qq
#安装qt依赖库
sudo apt-get install build-essential -y
sudo apt-get install mesa-common-dev -y
sudo apt-get install libglu1-mesa-dev -y
sudo apt-get install libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev -y
sudo apt-get install -y libicu-dev 
#sudo apt-get install -y qtbase5-dev qtmultimedia5-dev qttools5-dev-tools
sudo apt-get install -y libspeex-dev libtheora-dev libvpx-dev
sudo apt-get install -y libx264-dev 
#sudo apt-get remove -y libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev
sudo apt-get install -y libopencv-dev
#sudo apt-get install -y libqxmpp-dev
sudo apt-get install -y libssl-dev
sudo apt-get install -y libcurl4-openssl-dev
sudo apt-get install -y yasm
sudo apt-get install -y doxygen
#sudo apt-get autoremove
