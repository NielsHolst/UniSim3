echo on
set PATH=C:\MyDocuments\QDev\UniSim3\ship\cmd\ig\

cd %PATH%
pause

# Set version and target platform (32 or 64 bits)
set VERSION=3.0.32
REM set QT_PATH=C:\Qt\5.15.2\mingw81_32
set QT_PATH=C:\Qt\6.4.2\mingw_64

set IGLIB_BIN=iglib\bin
set IGLIB_BIN_IMAGEFORMATS=iglib\bin\imageformats
set IGLIB_BIN_PLATFORMS=iglib\bin\platforms
set IGLIB_BIN_PLUGINS=iglib\bin\plugins

set IGLIB_BIN=%PATH%%IGLIB_BIN%
set IGLIB_BIN_IMAGEFORMATS=%PATH%%IGLIB_BIN_IMAGEFORMATS%
set IGLIB_BIN_PLATFORMS=%PATH%%IGLIB_BIN_PLATFORMS%
set IGLIB_BIN_PLUGINS=%PATH%%IGLIB_BIN_PLUGINS%

rmdir /S /Q iglib
mkdir iglib
pushd iglib
mkdir bin
pushd bin
mkdir imageformats
mkdir platforms
mkdir plugins
popd
popd

echo = Copy iglib.h =
copy ..\..\..\src\lib\iglib\iglib.h iglib

echo = Copy UniSim plug-ins =
copy ..\..\..\bin\plugins\*.dll %IGLIB_BIN_PLUGINS%

echo = Copy UniSim libraries =
copy ..\..\..\bin\iglib.dll %IGLIB_BIN%
copy ..\..\..\bin\libiglib.a %IGLIB_BIN%
copy ..\..\..\bin\universal_simulator_base.dll %IGLIB_BIN%

echo = Copy Qt library folders =
pushd %QT_PATH%
pause

copy plugins\imageformats\*.* %IGLIB_BIN_IMAGEFORMATS%
copy plugins\platforms\*.* %IGLIB_BIN_PLATFORMS%

echo = Copy Qt libraries =
copy bin\Qt6Core.dll %IGLIB_BIN%
copy bin\Qt6Gui.dll %IGLIB_BIN%
copy bin\Qt6Network.dll %IGLIB_BIN%
copy bin\Qt6PrintSupport.dll %IGLIB_BIN%
copy bin\Qt6Test.dll %IGLIB_BIN%
copy bin\Qt6Widgets.dll %IGLIB_BIN%
copy bin\Qt6Xml.dll %IGLIB_BIN%
pause 

echo = Copy C++ libraries =
copy "bin\libstdc++-6.dll" %IGLIB_BIN%
copy bin\libwinpthread-1.dll %IGLIB_BIN%
popd

REM echo = Copy Windows libraries
REM pushd C:\Windows\SysWOW64
REM copy atl.dll %IGLIB_BIN%
REM copy comctl32.dll %IGLIB_BIN%
REM copy IPHLPAPI.dll %IGLIB_BIN%
REM copy rpcrt4.dll %IGLIB_BIN%
REM copy msvcrt.dll %IGLIB_BIN%
REM copy ntdll.dll %IGLIB_BIN%
REM copy oleaut32.dll %IGLIB_BIN%
REM copy bcryptprimitives.dll %IGLIB_BIN%
popd

set A=iglib_
set DIRNAME=%A%%VERSION%
ren iglib %DIRNAME%
pause