export QTDIR=/opt/Qt/6.5.3/android_armv7
export MAKEDIR=$ANDROID_NDK_ROOT/prebuilt/linux-x86_64/bin
export ANDROID_SDK_ROOT=/root/Android/Sdk
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64

#
# rara
#
mkdir -p setup
mkdir -p build-
cd build-
$QTDIR/bin/qmake ../rara.pro -spec android-clang CONFIG+=release
$MAKEDIR/make -j$(nproc)
$MAKEDIR/make INSTALL_ROOT=./android-build install
$QTDIR/../gcc_64/bin/androiddeployqt --input android-rara-deployment-settings.json --output ./android-build --android-platform android-31 --jdk /usr/lib/jvm/java-11-openjdk-amd64 --gradle
cp android-build/build/outputs/apk/debug/android-build-debug.apk ../setup/rara-$(sed 's/"//g' ../version.txt).apk
cd ..

