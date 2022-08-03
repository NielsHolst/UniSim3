if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bin\testd.exe" (
  "%cur_dir%\bin\testd.exe" > "%cur_dir%\output\ud.txt" 2> "%cur_dir%\output\ud2.txt"
) else (
  "%cur_dir%\bin\test.exe"  > "%cur_dir%\output\ud.txt" 2> "%cur_dir%\output\ud2.txt"
)
@echo Finished
pause
