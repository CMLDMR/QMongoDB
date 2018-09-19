#-------------------------------------------------
#
# Project created by QtCreator 2018-08-02T10:58:59
#
#-------------------------------------------------

QT       -= gui
QT       += core network qml

TARGET = QMongoDB
TEMPLATE = lib

CONFIG += c++14
API_VER=0.6


DEFINES += QMONGODB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        qmongodb.cpp \
    qbson.cpp \
    qerror.cpp \
    src/socket.cpp \
    qelement.cpp \
    qml/qmlmongodb.cpp \
    qml/qmlelement.cpp \
    qml/qmlbson.cpp \
    qml/qmlarray.cpp

HEADERS += \
        qmongodb.h \
        qmongodb_global.h \
    qbson.h \
    qerror.h \
    src/socket.h \
    qelement.h \
    qml/qmlmongodb.h \
    qml/qmlelement.h \
    qml/qmlbson.h \
    qml/qmlarray.h




qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



android{

    equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
        message("* using settings for android - armeabi-v7a");

        LIBS += -L$$PWD/android/lib/ -lbson-1.0 -lmongoc-1.0

        INCLUDEPATH += $$PWD/android/include/libbson-1.0

        DEPENDPATH += $$PWD/android/include/libbson-1.0

        INCLUDEPATH += $$PWD/android/include/libmongoc-1.0

        DEPENDPATH += $$PWD/android/include/libmongoc-1.0
    }


    equals(ANDROID_TARGET_ARCH, armeabi) {
        message("* using settings for android - armeabi");
    }
    equals(ANDROID_TARGET_ARCH, x86)  {
        message("* using settings for android - x86");
    }


}


win32{

    gcc{
        message("* using settings for windows gcc 5.30");
        LIBS += -L$$PWD/mingw32/lib/ -llibbson-1.0.dll -llibmongoc-1.0.dll


        INCLUDEPATH += $$PWD/mingw32/include/libbson-1.0
        DEPENDPATH += $$PWD/mingw32/include/libbson-1.0

        INCLUDEPATH += $$PWD/mingw32/include/libmongoc-1.0
        DEPENDPATH += $$PWD/mingw32/include/libmongoc-1.0

        INCLUDEPATH += $$PWD/mingw32/include
        DEPENDPATH += $$PWD/mingw32/include
    }

    msvc{
        message("* using settings for windows msvc2015x64");

        LIBS += -L$$PWD/msvc2015x64/lib/ -lbson-1.0 -lmongoc-1.0

        INCLUDEPATH += $$PWD/msvc2015x64/include/libbson-1.0
        DEPENDPATH += $$PWD/msvc2015x64/include/libbson-1.0

        INCLUDEPATH += $$PWD/msvc2015x64/include/libmongoc-1.0
        DEPENDPATH += $$PWD/msvc2015x64/include/libmongoc-1.0

    }


}


mac{
    message("* using settings for mac");
QT       += network

}

ios{
    message("* using settings for ios");
QT       += network
}

DISTFILES += \
    README.md \
    qmldir

RESOURCES +=



