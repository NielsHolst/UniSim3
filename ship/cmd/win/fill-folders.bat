set UNISIM_SHIP=%DOCUMENTS%\QDev\UniSim3\ship
set UNISIM_BIN=%UNISIM_SHIP%\bin
if not exist %UNISIM_SHIP% (
  echo %UNISIM_SHIP% does not exist
  echo Please set the DOCUMENTS variable
  goto :end_fail
)

set QT_ROOT=C:\Qt\5.15.1\mingw81_64
if exist %QT_ROOT% goto :ok_qt
set QT_ROOT=C:\Qt\Qt5.11.2\5.11.2\mingw53_32
if exist %QT_ROOT% goto :ok_qt
set QT_ROOT=C:\Qt\5.11.2\mingw53_32
if exist %QT_ROOT% goto :ok_qt
set QT_ROOT=C:\Qt\5.12.1\mingw73_64
if exist %QT_ROOT% goto :ok_qt
set QT_ROOT=C:\Qt\5.9.2\mingw53_32
if exist %QT_ROOT% goto :ok_qt
echo .
echo Could not find %QT_ROOT%
echo .
goto :end_fail


:ok_qt
set QT_BIN=%QT_ROOT%\bin
set QT_PLUGINS=%QT_ROOT%\plugins

echo = Clean target bin  =
rd /Q /S %UNISIM_BIN%
md %UNISIM_BIN%
md %UNISIM_BIN%\plugins

echo = Copy MS files =
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%
copy comctl32.dll %UNISIM_BIN%
copy mfc42u.dll %UNISIM_BIN%
copy msvcrt.dll %UNISIM_BIN%
copy oleaut32.dll %UNISIM_BIN%
popd

echo = Copy icon file =
copy %UNISIM_SHIP%\builder-input\*.ico %UNISIM_BIN%

echo = Copy exe and DLL files =
copy %UNISIM_SHIP%\..\bin\unisim.exe %UNISIM_BIN%
copy %UNISIM_SHIP%\..\bin\universal_simulator_base.dll %UNISIM_BIN%
copy %UNISIM_SHIP%\..\bin\plugins\*.dll %UNISIM_BIN%\plugins

echo = AltovaXML =
copy %UNISIM_SHIP%\..\..\AltovaXML.exe %UNISIM_BIN%

echo = Copy MinGW libraries =
pushd %QT_BIN%
copy icu*.dll %UNISIM_BIN%\
copy libwinpthread-1.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy libgcc_s_seh-1.dll %UNISIM_BIN%
copy libstd*.dll %UNISIM_BIN%

echo = Copy Qt libraries =
copy qt5core.dll %UNISIM_BIN%
copy qt5gui.dll %UNISIM_BIN%
copy Qt5Network.dll %UNISIM_BIN%
copy Qt5PrintSupport.dll %UNISIM_BIN%
copy qt5test.dll %UNISIM_BIN%
copy Qt5Widgets.dll %UNISIM_BIN%
copy qt5xmlpatterns.dll %UNISIM_BIN%
copy Qt5WinExtras.dll %UNISIM_BIN%
popd
echo = Create Qt plugin folders =
pushd %UNISIM_BIN%
md imageformats
md platforms
popd

echo = Copy Qt plugin folders =
pushd %QT_PLUGINS%
copy imageformats %UNISIM_BIN%\imageformats
copy platforms %UNISIM_BIN%\platforms
popd

rem Success
exit /B 0

:end_fail
@echo *** BUILD FAILURE ***
exit /B 1

