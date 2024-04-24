rmdir /S /Q site\input
mkdir site\input
mkdir site\input\demo
mkdir site\input\models
mkdir site\input\scripts

xcopy /S input\demo\*.* site\input\demo
xcopy /S input\models\*.* site\input\models
xcopy /S input\scripts\*.* site\input\scripts
pause
