SOURCES += main.cpp\
        MainWindow.cpp \
    Widgets/DlgLogin/DlgLogin.cpp \
    Widgets/DlgLogin/FrmLogin.cpp \
    XmppClient.cpp \
    Widgets/FrmUserList/FrmUserList.cpp \
    Widgets/FrmUserList/Roster.cpp \
    Widgets/FrmUserList/GroupItem.cpp \
    Widgets/FrmUserList/RosterItem.cpp

HEADERS  += MainWindow.h \
    Widgets/DlgLogin/DlgLogin.h \
    Widgets/DlgLogin/FrmLogin.h \
    XmppClient.h \
    Widgets/FrmUserList/FrmUserList.h \
    Widgets/FrmUserList/Roster.h \
    Widgets/FrmUserList/GroupItem.h \
    Widgets/FrmUserList/RosterItem.h

FORMS    += MainWindow.ui \
    Widgets/DlgLogin/DlgLogin.ui \
    Widgets/DlgLogin/FrmLogin.ui \
    Widgets/FrmUserList/FrmUserList.ui

RESOURCES += \
    Resource/Resource.qrc
