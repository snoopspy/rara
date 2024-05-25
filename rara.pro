QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
DESTDIR = $$PWD/bin

SOURCES += \
    gtrace.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    gtrace.h \
    widget.h

FORMS += \
    widget.ui

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	ANDROID_PACKAGE_SOURCE_DIR = \
		$$PWD/android
}
