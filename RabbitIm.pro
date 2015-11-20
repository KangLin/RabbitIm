TEMPLATE = subdirs

RabbitImLib.file = RabbitImLib.pro
RabbitImApp.file = RabbitImApp.pro
#RabbitImApp.depends = RabbitImLib
#Plugin.depends = RabbitImLib

CONFIG *= ordered
SUBDIRS +=  RabbitImLib \
    RabbitImApp \
    Plugin

# Rules for creating/updating {ts|qm}-files
include(Resource/translations/translations.pri)
# Build all the qm files now, to make RCC happy
#system($$fromfile(Resource/translations/translations.pri, updateallqm))
