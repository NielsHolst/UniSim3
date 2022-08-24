#!/bin/sh
echo = Create App bundle =
target=../../../bin/unisim.app
~/Qt/6.3.1/macos/bin/macdeployqt $target -always-overwrite -libpath=/users/nielsholst/lib

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

