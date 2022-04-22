QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileServer.cpp \
    NPSWrap.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FileServer.h \
    NPSWrap.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/package/ZLMediaKit_x86.1.0.0.2/build/native/lib/Win32/v140/dynamic/release/ -lmk_api
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/package/ZLMediaKit_x86.1.0.0.2/build/native/lib/Win32/v140/dynamic/debug/ -lmk_api

INCLUDEPATH += $$PWD/package/ZLMediaKit_x86.1.0.0.2/build/native/include
DEPENDPATH += $$PWD/package/ZLMediaKit_x86.1.0.0.2/build/native/include

RESOURCES += \
    resourecs.qrc

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
