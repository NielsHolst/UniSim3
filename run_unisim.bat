set cur_dir=%cd%
call %QT_ROOT_32%\qtenv2.bat
if exist "%cur_dir%\bin\unisimd.exe" (
  "%cur_dir%\bin\unisimd.exe"
) else (
  "%cur_dir%\bin\unisim.exe"
)
cd %cur_dir%