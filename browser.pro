QT       += core gui
QT += webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += headers
SOURCES += \
    src/clickablelineedit.cpp \
    src/header.cpp \
    src/interceptor.cpp \
    src/interceptorsettings.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/qpoint_helper.cpp \
    src/settings.cpp \
    src/util.cpp \
    src/webengineview.cpp \
    src/widget.cpp \
    src/MenuHelper.cpp
	

HEADERS += \
    headers/clickablelineedit.h \
    headers/header.h \
    headers/interceptor.h \
    headers/interceptorsettings.h \
    headers/mainwindow.h \
    headers/qpoint_helper.h \
    headers/settings.h \
    headers/util.h \
    headers/webengineview.h \
    headers/widget.h \
    src/MenuHelper.h

FORMS += \
    header.ui \
    mainwindow.ui \
    testing.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
