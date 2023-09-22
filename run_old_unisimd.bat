if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bind_old\unisimd.exe" (
  "%cur_dir%\bind_old\unisimd.exe"
) else (
  "%cur_dir%\bind\unisim.exe"
)
cd %cur_dir%