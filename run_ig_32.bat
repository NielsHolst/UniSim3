if not exist output mkdir output
@echo Run client...
set cur_dir=%cd%
call %QT_ROOT_32%\qtenv2.bat
if exist "%cur_dir%\bin\igclientd.exe" (
  "%cur_dir%\bin\igclientd.exe"
) else (
  "%cur_dir%\bin\igclient.exe"
)
@echo Finished
pause