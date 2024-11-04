if not exist output mkdir output
set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
@echo Run client...
if exist "%cur_dir%\bind\igclientd.exe" (
  "%cur_dir%\bind\igclientd.exe"
) else (
  "%cur_dir%\bin\igclient.exe"
)
cd %cur_dir%
@echo Finished
pause