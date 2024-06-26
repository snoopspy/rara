QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
DESTDIR = $$PWD/bin

SOURCES += \
    about.cpp \
    arch.cpp \
    gcheckbox.cpp \
    gsignal.cpp \
    gtablewidget.cpp \
    gtrace.cpp \
    log.cpp \
    option.cpp \
    packages.cpp \
    process.cpp \
    rara.cpp \
    widget.cpp \
    zygote.cpp

HEADERS += \
    about.h \
    arch.h \
    gcheckbox.h \
    gitemdelegate.h \
    gsignal.h \
    gtablewidget.h \
    gtrace.h \
    ini.h \
    log.h \
    option.h \
    packages.h \
    process.h \
    widget.h \
    zygote.h

FORMS += \
    about.ui \
    log.ui \
    widget.ui

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
	ANDROID_PACKAGE_SOURCE_DIR = \
		$$PWD/android
}

DISTFILES += \
	android/AndroidManifest.xml \
	android/build.gradle \
	android/gradle.properties \
	android/gradle/wrapper/gradle-wrapper.jar \
	android/gradle/wrapper/gradle-wrapper.properties \
	android/gradlew \
	android/gradlew.bat \
	android/res/drawable-hdpi/icon.png \
	android/res/drawable-ldpi/icon.png \
	android/res/drawable-mdpi/icon.png \
	android/res/drawable-xhdpi/icon.png \
	android/res/drawable-xxhdpi/icon.png \
	android/res/drawable-xxxhdpi/icon.png \
	android/res/values/libs.xml

RESOURCES += \
	rara.qrc

