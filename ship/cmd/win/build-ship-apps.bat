pushd ../..
qmake UniSim3.pro
if errorlevel 1 goto :end_fail
mingw32-make
if errorlevel 1 goto :end_fail

exit /B 0
:end_fail
@echo *** BUILD FAILURE ***
exit /B 1
