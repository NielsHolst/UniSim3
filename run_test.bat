if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bind\testd.exe" (
  "%cur_dir%\bind\testd.exe" > "%cur_dir%\output\ud.txt"
) else (
  "%cur_dir%\bin\test.exe" > "%cur_dir%\output\ud.txt"
)
cd %cur_dir%
