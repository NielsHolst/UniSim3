set cur_dir=%cd%
call %QT_ROOT_32%\qtenv2.bat
if exist "%cur_dir%\bin32\unisimd.exe" (
  "%cur_dir%\bin32\unisimd.exe"
) else (
  "%cur_dir%\bin32\unisim.exe"
)