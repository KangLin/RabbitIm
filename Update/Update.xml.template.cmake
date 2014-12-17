<?xml version="1.0" encoding="UTF-8"?>

<!--
每个操作系统一个文件
version：当前版本号
time:当前版本生成的时间
info：当前版本相关信息
force：是否强制更新，如果此标志为true，则忽略 min_compatible_version ,强制更新
min_compatible_version:最小兼容版本，小于此版本必须更新
url：当前版本下载地址
md5sum:当前下载文件的md5校验和
operation_system:操作系统 Platform:硬件平台 Architecture:硬件架构
-->

<RABBITIM>
    <SYSTEM>${RABBITIM_SYSTEM}</SYSTEM>
    <PLATFORM>${RABBITIM_PLATFORM}</PLATFORM>
    <ARCHITECTURE>${RABBITIM_ARCHITECTURE}</ARCHITECTURE>
    <MAJOR_VERSION_NUMBER>${MAJOR_VERSION_NUMBER}</MAJOR_VERSION_NUMBER>
    <MINOR_VERSION_NUMBER>${MINOR_VERSION_NUMBER}</MINOR_VERSION_NUMBER>
    <REVISION_VERSION_NUMBER>${REVISION_VERSION_NUMBER}</REVISION_VERSION_NUMBER>
    <TIME>${RABBITIM_TIME}</TIME>
    <INFO>${RABBITIM_INFO}</INFO>
    <FORCE>${RABBITIM_FORCE}</FORCE>
    <URL>${RABBITIM_UPDATE_CURL}</URL>
    <MD5SUM>${RABBITIM_MD5SUM}</MD5SUM>
    <MIN_COMPATIBLE_VERSION>${RABBITIM_MIN_COMPATIBLE_VERSIOIN}</MIN_COMPATIBLE_VERSION>
</RABBITIM>
