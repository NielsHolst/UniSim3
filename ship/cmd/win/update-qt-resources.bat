rmdir /S /Q ..\..\..\src\resources\data
mkdir ..\..\..\src\resources\data\input
xcopy /S /Q /I ..\..\..\input\demo ..\..\..\src\resources\data\input\demo
xcopy /S /Q /I ..\..\..\input\models ..\..\..\src\resources\data\input\models
xcopy /S /Q /I ..\..\..\input\scripts ..\..\..\src\resources\data\input\scripts
