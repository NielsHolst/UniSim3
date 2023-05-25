if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bin\unisim.exe" (
  "%cur_dir%\bin\unisim.exe"
) else (
  echo "Cannot find unisim.exe"
)
cd %cur_dir%