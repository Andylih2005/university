QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bookmode.cpp \
    changebook.cpp \
    checkbook.cpp \
    dlg_changepassword.cpp \
    dlg_signup.cpp \
    dlg_welcome.cpp \
    findbook.cpp \
    inputbook.cpp \
    main.cpp \
    paymode.cpp \
    personal.cpp \
    sqlite3.c \
    usemmanagement.cpp \
    user_center.cpp

HEADERS += \
    bookmode.h \
    changebook.h \
    checkbook.h \
    dlg_changepassword.h \
    dlg_signup.h \
    dlg_welcome.h \
    findbook.h \
    inputbook.h \
    paymode.h \
    personal.h \
    sqlite3.h \
    usemmanagement.h \
    user_center.h

FORMS += \
    bookmode.ui \
    changebook.ui \
    checkbook.ui \
    dlg_changepassword.ui \
    dlg_signup.ui \
    dlg_welcome.ui \
    findbook.ui \
    inputbook.ui \
    paymode.ui \
    personal.ui \
    usemmanagement.ui \
    user_center.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Logo.qrc

SUBDIRS += \
    ../MainBMS/BookManagementSystem.pro

DISTFILES +=

QMAKE_PROJECT_DEPTH=0
