if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT_32%\qtenv2.bat
if exist "%cur_dir%\bin32\testd.exe" (
  "%cur_dir%\bin32\testd.exe" > "%cur_dir%\output\ud.txt" 2> "%cur_dir%\output\ud2.txt"
) else (
  "%cur_dir%\bin32\test.exe"  > "%cur_dir%\output\ud.txt" 2> "%cur_dir%\output\ud2.txt"
)
cd %cur_dir%
@echo Finished
pause
