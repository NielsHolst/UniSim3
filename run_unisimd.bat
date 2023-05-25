if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bind\unisimd.exe" (
  "%cur_dir%\bind\unisimd.exe"
) else (
  "%cur_dir%\bind\unisim.exe"
)
cd %cur_dir%