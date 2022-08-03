# File suffix (debug or release)
suffix=d
echo $suffix

# Clean target bin and create bin sub-folders
rm -rf bin
mkdir bin
mkdir bin/lib
mkdir bin/lib/imageformats
mkdir bin/lib/platforms
mkdir bin/plugins

# Copy icon file
cp *.ico bin

# Copy system libs
cp /usr/lib/i386-linux-gnu/libstdc++.so.6 		bin/lib
cp /usr/lib/i386-linux-gnu/libgobject-2.0.so.0 		bin/lib
cp /usr/lib/i386-linux-gnu/libgthread-2.0.so.0 		bin/lib
cp /usr/lib/i386-linux-gnu/libXext.so.6   		bin/lib
cp /usr/lib/i386-linux-gnu/libX11.so.6    		bin/lib
cp /usr/lib/i386-linux-gnu/mesa/libGL.so.1 		bin/lib
cp /lib/i386-linux-gnu/libgcc_s.so.1      		bin/lib
cp /lib/i386-linux-gnu/libc.so.6          		bin/lib
cp /lib/i386-linux-gnu/libpthread.so.0    		bin/lib
cp /lib/i386-linux-gnu/librt.so.1 	  		bin/lib
cp /lib/i386-linux-gnu/libglib-2.0.so.0   		bin/lib
cp /lib/i386-linux-gnu/libm.so.6 	  		bin/lib

# Copy UniSim executable and libs
cp ../bin/unisim$suffix bin
cp ../bin/libbase$suffix* bin/lib

# Copy UniSim plugins
cp ../bin/plugins/lib*.so bin/plugins

# Copy gcc libs
cp ~/Qt5.5.1/5.5/gcc/lib/libicui18n.so.54 	bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libicuuc.so.54 	bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libicudata.so.54	bin/lib

# Copy Qt libraries
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5Core.so.5 		bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5Widgets.so.5 		bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5Gui.so.5 		bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5PrintSupport.so.5	bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5XmlPatterns.so.5	bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5Network.so.5		bin/lib
cp ~/Qt5.5.1/5.5/gcc/lib/libQt5Test.so.5.5		bin/lib

# Copy Qt plugin folders
cp ~/Qt5.5.1/5.5/gcc/plugins/imageformats/* bin/lib/imageformats
cp ~/Qt5.5.1/5.5/gcc/plugins/platforms/* bin/lib/platforms

echo "END"

