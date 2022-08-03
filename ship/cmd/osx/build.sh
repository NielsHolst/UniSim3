#!/bin/sh
MAJOR=2
MINOR=3
SUB=10
VERSION=release
echo
echo Building $MAJOR.$MINOR.$SUB $VERSION
echo

echo = Clean out old builds =
pushd ../../..
./clean.sh
popd

echo = Build ship apps =
rm -r ../bin
pushd ../src
~/Qt/5.9.1/clang_64/bin/qmake ship-cmd.pro
make -B
popd

echo = Update Qt resources
./update-qt-resources.sh
../bin/update-qt-resources $MAJOR $MINOR $SUB $VERSION

echo = Update Qt project
../bin/update-qt-project $MAJOR $MINOR $SUB $VERSION

echo = Build Universal Simulator =
pushd ../../..
~/Qt/5.9.1/clang_64/bin/qmake UniSim2.pro -spec ~/Qt/5.9.1/clang_64/mkspecs/macx-clang CONFIG+=x86_64
make -B
popd	

echo = Create App bundle =
target=../../../bin/unisim.app
~/Qt/5.9.1/clang_64/bin/macdeployqt $target -always-overwrite -libpath=/users/nielsholst/lib

echo = Remove base library from bundle so that it only occurs in ~/lib
rm $target/Contents/Frameworks/*.dylib

echo = Copy UniSim plugins =
mkdir $target/Contents/MacOS/plugins
cp ../../../bin/plugins/*.dylib $target/Contents/MacOS/plugins
otool -L  $target/Contents/MacOS/unisim$suffix

echo = Set installer version number =
../bin/update-installer-project $MAJOR $MINOR $SUB $VERSION

echo = Restore to debug version =
../bin/update-qt-project $MAJOR $MINOR $SUB debug

#!/bin/bash +vx
echo "END"

