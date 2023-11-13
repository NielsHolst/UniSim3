echo on
if exist D:\Documents\QDev\UniSim3\ship\cmd\win goto :d_drive
if exist C:\MyDocuments\QDev\UniSim3\ship\cmd\win goto :c_drive
echo Could not find ship\cmd\win folder
goto :end_fail
:d_drive
D:
cd \Documents\QDev\UniSim3\ship\cmd\win
goto :ok_cmd_folder
:c_drive
C:
cd \MyDocuments\QDev\UniSim3\ship\cmd\win
:ok_cmd_folder

rem Set up version
set MAJOR=3
set MINOR=0
set SUB=18
set VERSION=release
echo Building version %MAJOR%.%MINOR%.%SUB% %VERSION%

echo = Set path to Qt build environment =

set QTENV="C:\Qt\6.4.2\mingw_64\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt

echo .
echo Could not find %QTENV%
echo .
goto :end_fail

:ok_qt 
echo = Set path to InstallBuilder =
set INSTALL_BUILDER_PATH="C:\Program Files (x86)\BitRock InstallBuilder for Qt 17.7.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder

set INSTALL_BUILDER_PATH="C:\Program Files\BitRock InstallBuilder for Qt 16.7.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder

set INSTALL_BUILDER_PATH="C:\Program Files (x86)\BitRock InstallBuilder for Qt 16.8.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder
echo .
echo Could not find %INSTALL_BUILDER_PATH%
echo .
REM goto :end_fail until InstallBuilder is installed!!!!
:ok_builder

rem Call Qt batch file 
pushd .
call %QTENV%
popd

echo = Clean out old builds =
pushd ..\..\..
call clean
popd

echo = Build ship apps =
del /S /Q ..\bin
pushd ..\src
qmake ship-cmd.pro
if errorlevel 1 goto :end_fail
mingw32-make
if errorlevel 1 goto :end_fail
popd

echo = Update Qt resources =
call update-qt-resources.bat 
if errorlevel 1 goto :end_fail
..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% %VERSION%

echo = Update Qt project  =
..\bin\update-qt-project %MAJOR% %MINOR% %SUB% %VERSION%
if errorlevel 1 goto :end_fail

echo = Build Universal Simulator =
pushd ..\..\..
qmake UniSim3.pro
if errorlevel 1 goto :end_fail
mingw32-make -B
if errorlevel 1 goto :end_fail
popd

echo = Copy binary files =
call fill-folders
if errorlevel 1 goto :end_fail

echo = Set installer version number =
..\bin\update-installer-project %MAJOR% %MINOR% %SUB%
if errorlevel 1 goto :end_fail

REM echo = Build installer =
REM pushd %INSTALL_BUILDER_PATH%
REM builder build \data\QDev\UniSim3\ship\UniSim3_install_builder.xml
REM popd

REM echo = Restore to debug version =
REM ..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% debug
REM ..\bin\update-qt-project %MAJOR% %MINOR% %SUB% debug

echo *** SUCCESS ***
pause
exit
:end_fail
echo *** BUILD FAILURE ***
echo on
pause
