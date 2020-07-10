#!/bin/bash

# Use to install appimage in linux
# $1: install or remove
# $2: run

case "$1" in
    remove)
        echo "remove ..."
        rm -f /usr/share/applications/RabbitIm.desktop
        rm -f ~/.config/autostart/RabbitIm.desktop
        rm -f /usr/share/pixmaps/RabbitIm.png
    ;;

    install|*)
        echo "install ..."
        # Install destop
        if [ -f /usr/share/applications/RabbitIm.desktop ]; then
            rm /usr/share/applications/RabbitIm.desktop
        fi
        ln -s `pwd`/share/applications/RabbitIm.desktop /usr/share/applications/RabbitIm.desktop

        # Install auto run on boot
        if [ ! -d ~/.config/autostart ]; then
            mkdir -p ~/.config/autostart
            chmod a+wr ~/.config/autostart
        fi
        if [ -f ~/.config/autostart/RabbitIm.desktop ]; then
            rm ~/.config/autostart/RabbitIm.desktop
        fi
        ln -s `pwd`/share/applications/RabbitIm.desktop ~/.config/autostart/RabbitIm.desktop

        # Reset exec to desktop
        sed -i "s/Exec=.*//g" `pwd`/share/applications/RabbitIm.desktop
        echo "Exec=`pwd`/RabbitIm-x86_64.AppImage" >> `pwd`/share/applications/RabbitIm.desktop

        # Install applications icon
        if [ -f /usr/share/pixmaps/RabbitIm.png ]; then
            rm /usr/share/pixmaps/RabbitIm.png
        fi
        if [ ! -d /usr/share/pixmaps ]; then
            mkdir -p /usr/share/pixmaps
        fi
        ln -s `pwd`/share/pixmaps/RabbitIm.png /usr/share/pixmaps/RabbitIm.png
        
        # Whether run after install
        if [ "$2" = "run" ]; then
            ./RabbitIm-x86_64.AppImage
        fi
        ;;
esac
